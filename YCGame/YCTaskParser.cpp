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
	<t color='ffff0000'>���鳾�����������������ٵ�ʱ��˭���޷����ܣ�</t>
	</p>"
	1 = 'link' '�ָ��' 'dlg1'
	2 = 'server_cmd' '���ͻ��ͼ' '1'
	3 = 'link' '��ɱ���ѯ' 'dlg2'
	4 = 'link' '��ȡ�����' 'dlg3'
	5 = 'server_cmd' '�뿪' '2'
	[dlg1]
talk ="�ָ��"
	%s
	2 = 'link' '���񿼺���' 'dlg12'
	3 = 'link' '����' 'start'
	[dlg11]
talk =" ϵͳ���ڻ�ڼ��Զ���¼���н�ɫ��Ļ�ɱ��¼"
	1 = 'link' '����' 'start'
	[dlg12]
talk =" ϵͳ���ڻ�ڼ��Զ���¼"
	1 = 'link' '����' 'start'
	[dlg2]
talk =" ���а�"
	%s
	2 = 'server_cmd' '���񿼺���' '22'
	3 = 'link' '����' 'start'
	[dlg3]
talk =" �����"
	%s
	2 = 'server_cmd' '���񿼺���' '32'
	3 = 'link' '����' 'start'
	"""%(GetFormatString(1), GetFormatString(2), GetFormatString(3))
*/

//////////////////////////////////////////////////////////////////////////
// YCTaskParserHelper : �ڲ�������
//////////////////////////////////////////////////////////////////////////
class YCTaskParserHelper
{
public:

	//
	// �ڲ��ࣺ��ʾһ������
	// 	  [dlg11]
	//  talk =" ϵͳ���ڻ�ڼ��Զ���¼���н�ɫ��Ļ�ɱ��¼"
	//  	1 = 'link' '����' 'start'
	//
	// ˵������Ϊ�ڲ���stringstream��ʾ������ֻ��˳���ȡ��
	//       ����sectionName -��talk -��commands
	//
	// ע�⣺getline���������һ��\n, \r�ᱻȥ��
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
		// У�����ݺϷ���
		//
		bool validate() const
		{
			return true;
		}

		//
		// ȡ����������dlg11
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
		// ȡ��˵�����ݣ�������xml
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
				if (read < 2) //\n ����
				{
					return NULL;
				}

				// <t>\r\n\r\n</t> �������м�Ļ��з���
				while (buffer[read-2] != '"')
				{
					myStream.getline(buffer+read, length-read);
					
					if (myStream.fail() || myStream.bad() || myStream.eof())
					{
						throw YCException(1002, "YCTaskParser��ȡtalkʧ��");
					}

					read = strlen(buffer);
				}

				// �Ƿ�"����
				if (buffer[read-2] == '"')
				{
					// ȥ������\n
					char* startPos = strstr(buffer, "talk =\"");
					if (startPos != NULL)
					{
						startPos += 7; //strlen(talk =\")
						
						//ȥ������"\n �����ַ�
						buffer[read-2] = '\0'; 
						unsigned int length = buffer+read-2-startPos;

						if (*startPos == '<') //XML
						{
							// �滻\r Ϊ ESC������ᱻtinyxml2 skip��

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
						else //�ı�
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
		// ȡ����һ������
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
					throw YCException(1002, "YCTaskParser��ȡtalkʧ��");
				}

				length = strlen(buffer);

				// 1 = 'link' '����' 'start'
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
							throw YCException(1002, "nextCommand��ЧNPCѡ��", buffer);
						}

						if     (!actFound)
						{
							if (*(i+1) == '\0')
							{
								throw YCException(1002, "nextCommand��ЧNPCѡ��", buffer);
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
								throw YCException(1002, "nextCommand��ЧNPCѡ��", buffer);
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
							//	throw YCException(1002, "nextCommand��ЧNPCѡ��", buffer);
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
								throw YCException(1002, "nextCommand��ЧNPCѡ��", buffer);
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
							idx = i; // ���ֿ�ʼ
						}
					}
				}

				if (!(idxFound && actFound && optFound && cmdFound))
				{
					throw YCException(1002, "nextCommand��ЧNPCѡ��", buffer);
				}

				//TODO ...
				if (atoi(idx) == 0)
				{
					throw YCException(1002, "nextCommand��ЧNPC������", idx);
				}

				// ��Ϣ��Ԫֻ������ʾѡ�� ��ȡ�����
				result = new YCTextUnit(opt, strlen(opt));
				result->bindMouseClick(
					buildUnitAction(std::tr1::bind(YCTaskMessage::onClickCommand, startMessage, std::string(act), std::string(cmd))));

			} catch (...) {
				LOG_ERROR("YCTaskSection����NPCѡ�����" << buffer);
			}

			return result;
		}

		///////////////////////////////////////////////////////
		// Helper Function : XML ��ȡ t �ڵ�
		///////////////////////////////////////////////////////
		bool handleT(YCXmlReader& reader, YCIMessageUnit** header)
		{
			char* command = NULL;
			unsigned int color;

			reader.attr("color", color, NPC_CHAT_TEXT_COLOR)
				  .value(&command);

			// �滻ESC�ص�\r
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
		// �ַ�����
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
	// �Ƿ�����������
	//
	bool next()
	{
		return (position < myContent+myLength);
	}

	//
	// ˳��ȡ����һ�����򣬶�Ӧһ��YCIMessageUnit����
	// �� [ ���ڶ��� [
	//
	YCTaskSection nextSection()
	{		
		if (!next())
		{
			throw YCException(1002, "YCTaskParserHelper::nextSection��ʣ������");
		}
		
		const char* theEnd = myContent+myLength;
		while (position < theEnd && (*position == '\r' || *position == '\n'))
		{
			position += 1;
		}

		if (position >= theEnd)
		{
			throw YCException(1002, "YCTaskParserHelper::nextSection��������ʧ��");
		}

		if (*position != '[')
		{
			throw YCException(1002, "YCTaskParserHelper::nextSection��Ч����ͷ");
		}

		const char* startPos = position;

		position += 1; //������һ��[
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
	// ������ָ�룬��������������
	//
	const char* myContent;

	//
	// �������ĳ���
	//
	unsigned int myLength;

	//
	// ��ǰ������������λ�ã���ʼΪ��
	//
	const char* position;
};

//
// ������parse(const NET_TASK_MSG_NTF_T* msg)
//
// Ŀ�ģ����������ݰ������ڽ���Ϣ��Ԫ
//
YCTaskMessage* YCTaskParser::parse(const NET_TASK_MSG_NTF_T* msg)
{/*
	SMART_ASSERT(msg != NULL);

	if (msg == NULL)
	{
		throw YCException(2002, "YCTaskParser::parse��ϢΪ��");
	}

	if (msg->count == 0 || msg->message == NULL)
	{
		throw YCException(2002, "YCTaskParser::parse��Ϣ����Ϊ0");
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
			throw YCException(2002, "YCTaskParser::parse��Ч��Ϣ��");
		}

		char sectionName[TASK_SECTION_NAME];
		if (!section.sectionName(sectionName, TASK_SECTION_NAME))
		{
			LOG_WARNING("YCTaskParser::parse��ЧNPC�Ի���Ϣ, ����������");
		}
		else
		{
			YCIMessageUnit* talk = section.talk();
			if (talk == NULL)
			{
				LOG_WARNING("YCTaskParser::parse����NPC�Ի���Ϣtalkʧ��");
			}
			else
			{
				YCTaskMessage* theMessage = new YCTaskMessage(sectionName);
				YCTaskMessage* theStart = ((result != NULL) ? result : theMessage);

				// ȷ��[start]�����ͷ
				const char* sectionName = theStart->getSectionName();
				SMART_ASSERT(stricmp(sectionName, "start") == 0);
				if (stricmp(sectionName, "start") != 0)
				{
					throw YCException(2002, "YCTaskParser::parse����start����ͷ");
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