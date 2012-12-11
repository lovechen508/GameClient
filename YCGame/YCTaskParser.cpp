#include "YCTaskParser.h"

#include <memory>
#include <string>
#include <sstream>
#include <regex>

#include "YCTaskMessage.h"
#include "YCIUnitAction.h"

#include "YCTextUnit.h"
#include "YCImageUnit.h"

#include "YCConfig\YCXmlReader.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCScopeGuard.h"
#include "YCBasic\YCStringHelper.h"

#include "YCNetwork\protocol\YCPkg_0090_Npc.h"

/*
	msg = """
	[start]
talk ="<?xml version='1.0' encoding='GB2312'?>
	<p>
	<t color='ffff0000'>尘归尘，土归土！当死神降临的时候，谁都无法抗拒！</t>
	</p>"
	1 = 'link' '活动指引' 'dlg1'
	2 = 'server_cmd' '传送活动地图' '1'
	3 = 'link' '击杀榜查询' 'dlg2'
	4 = 'link' '领取活动奖励' 'dlg3'
	5 = 'server_cmd' '离开' '2'
	[dlg1]
talk ="活动指引"
	%s
	2 = 'link' '死神考核赛' 'dlg12'
	3 = 'link' '返回' 'start'
	[dlg11]
talk =" 系统将在活动期间自动记录所有角色间的击杀记录"
	1 = 'link' '返回' 'start'
	[dlg12]
talk =" 系统将在活动期间自动记录"
	1 = 'link' '返回' 'start'
	[dlg2]
talk =" 排行榜"
	%s
	2 = 'server_cmd' '死神考核赛' '22'
	3 = 'link' '返回' 'start'
	[dlg3]
talk =" 活动奖励"
	%s
	2 = 'server_cmd' '死神考核赛' '32'
	3 = 'link' '返回' 'start'
	"""%(GetFormatString(1), GetFormatString(2), GetFormatString(3))
*/

//////////////////////////////////////////////////////////////////////////
// YCTaskParserHelper : 内部助手类
//////////////////////////////////////////////////////////////////////////
class YCTaskParserHelper
{
public:

	//
	// 内部类：标示一个区域
	// 	  [dlg11]
	//  talk =" 系统将在活动期间自动记录所有角色间的击杀记录"
	//  	1 = 'link' '返回' 'start'
	//
	// 说明：因为内部用stringstream标示，所以只能顺序读取！
	//       即：sectionName -》talk -》commands
	//
	// 注意：getline读出来会多一个\n, \r会被去掉
	//
	class YCTaskSection
	{
	public:

		YCTaskSection(const char* content, unsigned int length)
			: myStream(std::string(content, length))
		{
			SMART_ASSERT(content != NULL);
			SMART_ASSERT(length > 0);
		}

		~YCTaskSection() {}

		//
		// 校验数据合法性
		//
		bool validate() const
		{
			return true;
		}

		//
		// 取得区域名：dlg11
		//
		bool sectionName(char* buffer, unsigned int length)
		{
			try 
			{
				char nameWithBrackets[TASK_SECTION_NAME+2] ={0};
				myStream.getline(nameWithBrackets, TASK_SECTION_NAME+2);
				int lengthWithBrackets = strlen(nameWithBrackets);
				if (lengthWithBrackets < 4) // []\n
				{
					return false;
				}

				if (nameWithBrackets[0] != '[' || nameWithBrackets[lengthWithBrackets-2] != ']')
				{
					return false;
				}

				memcpy(buffer, nameWithBrackets+1, lengthWithBrackets-3);
				buffer[lengthWithBrackets-3]='\0';
			}
			catch (...) 
			{
				return false;
			}

			return true;
		}

		//
		// 取得说话内容：可能是xml
		//
		YCIMessageUnit* talk()
		{
			YCIMessageUnit* result = NULL;

			try 
			{
				char buffer[1024] = {0};
				unsigned int length = 1024;
				myStream.getline(buffer, length);
				unsigned int read = strlen(buffer);
				if (read < 2) //\n 空行
				{
					return NULL;
				}

				// <t>\r\n\r\n</t> 可能在中间的换行返回
				while (buffer[read-2] != '"')
				{
					myStream.getline(buffer+read, length-read);
					
					if (myStream.fail() || myStream.bad() || myStream.eof())
					{
						throw YCException(1002, "YCTaskParser读取talk失败");
					}

					read = strlen(buffer);
				}

				// 是否"结束
				if (buffer[read-2] == '"')
				{
					// 去掉最后的\n
					char* startPos = strstr(buffer, "talk =\"");
					if (startPos != NULL)
					{
						startPos += 7; //strlen(talk =\")
						
						//去掉最后的"\n 两个字符
						buffer[read-2] = '\0'; 
						unsigned int length = buffer+read-2-startPos;

						if (*startPos == '<') //XML
						{
							// 替换\r 为 ESC，否则会被tinyxml2 skip掉

							for (char* i = startPos; i < (startPos+length); ++i)
							{
								if (*i == '\r')
								{
									*i = 0x1B;
								}
							}
							YCXmlReader reader(startPos, length);
							reader.foreach("t", YCXmlReader::YCForEachWrapper1<YCTaskSection, YCIMessageUnit*>(this, &YCTaskSection::handleT, &result));
						}
						else //文本
						{
							result = new YCTextUnit(startPos, length);
						}
					}
				}				
			} 
			catch (YCException& e) 
			{
				UNREFERENCED_PARAMETER(e);
			}

			return result;
		}

		//
		// 取得下一条命令
		//
		YCIMessageUnit* nextCommand(YCTaskMessage* startMessage)
		{
			YCIMessageUnit* result = NULL;
			
			char buffer[1024] = {0};
			unsigned int length = 1024;

			try {
				myStream.getline(buffer, length);
				
				if (myStream.fail() || myStream.bad() || myStream.eof())
				{
					throw YCException(1002, "YCTaskParser读取talk失败");
				}

				length = strlen(buffer);

				// 1 = 'link' '返回' 'start'
				bool idxFound = false; const char* idx = NULL;
				bool actFound = false; const char* act = NULL;
				bool optFound = false; const char* opt = NULL;
				bool cmdFound = false; const char* cmd = NULL;

				bool equalFound = false; // =

				for (char* i = buffer; *i != '\0'; ++i)
				{
					if (*i == '\n') break;

					if (*i == '\t' || *i == ' ')
					{
						if (!equalFound)
						{
							if (idx != NULL)
							{
								*i = '\0';
								idxFound = true;
							}
						}
						continue;
					}

					if (*i == '\'')
					{
						if (!equalFound)
						{
							throw YCException(1002, "nextCommand无效NPC选项", buffer);
						}

						if     (!actFound)
						{
							if (*(i+1) == '\0')
							{
								throw YCException(1002, "nextCommand无效NPC选项", buffer);
							}

							if (act == NULL)
							{
								act = i+1; //startPosition
							}
							else
							{
								*i = '\0'; 
								actFound = true;
							}
						}
						else if (!optFound)
						{
							if (*(i+1) == '\0')
							{
								throw YCException(1002, "nextCommand无效NPC选项", buffer);
							}
							
							if (opt == NULL)
							{
								opt = i+1; //startPosition
							}
							else
							{
								*i = '\0'; 
								optFound = true;
							}
						}
						else if (!cmdFound)
						{
							//if (*(i+1) == '\0')
							//{
							//	throw YCException(1002, "nextCommand无效NPC选项", buffer);
							//}

							if (cmd == NULL)
							{
								cmd = i+1; //startPosition
							}
							else
							{
								*i = '\0'; 
								cmdFound = true;
							}
						}
					}
					else if (*i == '=')
					{
						if (!idxFound)
						{
							if (idx == NULL)
							{
								throw YCException(1002, "nextCommand无效NPC选项", buffer);
							}

							*i = '\0'; 
							idxFound = true;
						}

						equalFound = true;
					}
					else if (*i >= 49 && *i <= 57) // 1 < *i < 9
					{
						if (idx == NULL)
						{
							idx = i; // 数字开始
						}
					}
				}

				if (!(idxFound && actFound && optFound && cmdFound))
				{
					throw YCException(1002, "nextCommand无效NPC选项", buffer);
				}

				//TODO ...
				if (atoi(idx) == 0)
				{
					throw YCException(1002, "nextCommand无效NPC命令编号", idx);
				}

				// 消息单元只保存显示选项 领取活动奖励
				result = new YCTextUnit(opt, strlen(opt));
				result->bindMouseClick(
					buildUnitAction(std::tr1::bind(YCTaskMessage::onClickCommand, startMessage, std::string(act), std::string(cmd))));

			} catch (...) {
				LOG_ERROR("YCTaskSection处理NPC选项出错" << buffer);
			}

			return result;
		}

		///////////////////////////////////////////////////////
		// Helper Function : XML 读取 t 节点
		///////////////////////////////////////////////////////
		bool handleT(YCXmlReader& reader, YCIMessageUnit** header)
		{
			char* command = NULL;
			unsigned int color;

			reader.attr("color", color, NPC_CHAT_TEXT_COLOR)
				  .value(&command);

			// 替换ESC回到\r
			for (char* i = command; *i != '\0'; ++i)
			{
				if (*i == 0x1B)
				{
					*i = '\r';
				}
			}

			YCTextUnit* unit = new YCTextUnit(command, strlen(command));
			unit->setColor(color);

			if (*header == NULL)
			{
				*header = unit;
			}
			else
			{
				YCIMessageUnit* last = *header;
				while (last->next() != NULL)
				{
					last = last->next();
				}
				last->append(unit);
			}

			SAFE_DELETE(command);

			return true;
		}

	private:

		//
		// 字符串流
		//
		std::istringstream myStream;

	};

public:

	YCTaskParserHelper(const char* content, unsigned int length)
		: myContent(content)
		, myLength(length)
		, position(content)
	{
		SMART_ASSERT(content != NULL);
		SMART_ASSERT(length > 0);
	}

	~YCTaskParserHelper() {}

	//
	// 是否解析区域完毕
	//
	bool next()
	{
		return (position < myContent+myLength);
	}

	//
	// 顺序取得下一个区域，对应一个YCIMessageUnit链表
	// 从 [ 到第二个 [
	//
	YCTaskSection nextSection()
	{		
		if (!next())
		{
			throw YCException(1002, "YCTaskParserHelper::nextSection无剩余区域");
		}
		
		const char* theEnd = myContent+myLength;
		while (position < theEnd && (*position == '\r' || *position == '\n'))
		{
			position += 1;
		}

		if (position >= theEnd)
		{
			throw YCException(1002, "YCTaskParserHelper::nextSection查找区域失败");
		}

		if (*position != '[')
		{
			throw YCException(1002, "YCTaskParserHelper::nextSection无效区域头");
		}

		const char* startPos = position;

		position += 1; //跳过第一个[
		unsigned int length = 1;

		while (position < theEnd)
		{
			if (*position == '[')
			{
				break;
			}
			
			++position;
			++length;
		}

		return YCTaskSection(startPos, length);
	}

private:

	// 
	// 内容区指针，不持有生命周期
	//
	const char* myContent;

	//
	// 内容区的长度
	//
	unsigned int myLength;

	//
	// 当前解析到的命令位置，初始为空
	//
	const char* position;
};

//
// 函数：parse(const NET_TASK_MSG_NTF_T* msg)
//
// 目的：从网络数据包构建内建消息单元
//
YCTaskMessage* YCTaskParser::parse(const NET_TASK_MSG_NTF_T* msg)
{/*
	SMART_ASSERT(msg != NULL);

	if (msg == NULL)
	{
		throw YCException(2002, "YCTaskParser::parse消息为空");
	}

	if (msg->count == 0 || msg->message == NULL)
	{
		throw YCException(2002, "YCTaskParser::parse消息长度为0");
	}

	YCTaskMessage* result = NULL;
	YCTaskMessage* theEnd = NULL;
	SCOPE_GUARD(result_guard, delete result);

	YCTaskParserHelper helper(msg->message, msg->count);
	while (helper.next())
	{
		YCTaskParserHelper::YCTaskSection section = helper.nextSection();

		if (!section.validate())
		{
			throw YCException(2002, "YCTaskParser::parse无效消息包");
		}

		char sectionName[TASK_SECTION_NAME];
		if (!section.sectionName(sectionName, TASK_SECTION_NAME))
		{
			LOG_WARNING("YCTaskParser::parse无效NPC对话消息, 区域名有误");
		}
		else
		{
			YCIMessageUnit* talk = section.talk();
			if (talk == NULL)
			{
				LOG_WARNING("YCTaskParser::parse构建NPC对话消息talk失败");
			}
			else
			{
				YCTaskMessage* theMessage = new YCTaskMessage(sectionName);
				YCTaskMessage* theStart = ((result != NULL) ? result : theMessage);

				// 确保[start]区域打头
				const char* sectionName = theStart->getSectionName();
				SMART_ASSERT(stricmp(sectionName, "start") == 0);
				if (stricmp(sectionName, "start") != 0)
				{
					throw YCException(2002, "YCTaskParser::parse不是start区开头");
				}

				theMessage->setTalk(talk);

				YCIMessageUnit * command = NULL;
				while ((command = section.nextCommand(theStart)) != NULL)
				{
					command->onMouseClick();
					theMessage->setCommand(command);
				}

				if (result == NULL)
				{
					result = theMessage;
					theEnd = result;
				}
				else
				{
					theEnd->setNext(theMessage);
					theEnd = theMessage;
				}
			}
		}

	}

	SCOPE_GIVEUP(result_guard);

	return result;
*/
    return NULL;
}