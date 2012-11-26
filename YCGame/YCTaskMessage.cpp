#include "YCTaskMessage.h"

#include "YCIMessageUnit.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCTaskMessage::YCTaskMessage(const char* sectionName)
	: myNext(NULL)
	, myTalk(NULL)
	, myCommands(NULL)
{
	SMART_ASSERT(sectionName != NULL);

	int length = strlen(sectionName);
	if (length >= TASK_SECTION_NAME)
	{
		throw YCException(2002, "YCTaskMessage命令区域名称过长", length);
	}

	memcpy(mySectionName, sectionName, length);
	mySectionName[length] = '\0';
}


YCTaskMessage::~YCTaskMessage(void)
{
	SAFE_DELETE(myNext);
	SAFE_DELETE(myTalk);
	SAFE_DELETE(myCommands);
}

//
// 取得消息区域名
//
const char* YCTaskMessage::getSectionName() const
{
	return mySectionName;
}

//
// 注释：设置链表的下一条消息
//
void YCTaskMessage::setNext(YCTaskMessage* next)
{
	SMART_ASSERT(next != NULL);
	SMART_ASSERT(myNext == NULL);
	myNext = next;
}

//
// 注释：获取链表的下一条消息
//
YCTaskMessage* YCTaskMessage::getNext()
{
	return myNext;
}

//
// 注释：追加消息单元，一次追加对应一行消息
//
void YCTaskMessage::setTalk(YCIMessageUnit* unit)
{
	SMART_ASSERT(unit != NULL);
	SMART_ASSERT(myTalk == NULL);
	myTalk = unit;
}

//
// 注释：取得聊天消息单元
//
const YCIMessageUnit* YCTaskMessage::getTalk() const
{
	return myTalk;
}

//
// 设置命令消息，所有的命令消息组成一个链表
// 一个命令解析成一行，可以使图片链接，也可以使文字链接
//
void YCTaskMessage::setCommand(YCIMessageUnit* current)
{
	SMART_ASSERT(current != NULL);
	if (myCommands == NULL)
	{
		myCommands = current;
	}
	else
	{
		YCIMessageUnit* last = myCommands;
		while (last->next() != NULL)
		{
			last = last->next();
		}
		last->append(current);
	}
}

//
// 获取命令消息，返回链表的下一个消息单元
// 如果current为空，返回链表的第一个消息单元
//
const YCIMessageUnit* YCTaskMessage::getCommand() const
{
	return myCommands;
}

//
// 函数：onClickCommand(YCTaskMessage* message, const std::string& action, const std::string& command)
//
// 目的：绑定到消息单元，当消息单元点击的时候回调
//
// 参数：
//      Message : 消息单元所属的区的头部区[start], 用于command=dlg11时在YCTaskMessage链表中查找
//		Action	: 'link' 'server_cmd'
//      command	: 'dlg11' 23 ...
//
void YCTaskMessage::onClickCommand(YCTaskMessage* message, const std::string& action, const std::string& command)
{
	SMART_ASSERT(message != NULL);
	if (message == NULL)
	{
		throw YCException(1002, "YCTaskMessage::onClickCommand无效消息响应结构");
	}

	if (action == "link")
	{
		return;
	}
	else if (action == "server_cmd")
	{
		// 构建网络消息，发...
		return;
	}
	else
	{
		throw YCException(1002, "YCTaskMessage::onClickCommand未知Action", action.c_str());
	}
}