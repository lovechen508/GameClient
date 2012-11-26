#ifndef _INCLUDE_YCTASKMESSAGE_H_
#define _INCLUDE_YCTASKMESSAGE_H_

#include <string>

#include "YCTaskDefine.h"

class YCIMessageUnit;
struct YCIUnitAction;

//
// YCTaskMessage : 任务消息，
//     根据服务器的消息区，链接成链表结构
//
class YCTaskMessage
{
public:

	YCTaskMessage(const char* sectionName);

	~YCTaskMessage(void);

	//
	// 取得消息区域名
	//
	const char* getSectionName() const;

	//
	// 注释：设置链表的下一条消息
	//
	void setNext(YCTaskMessage* next);

	//
	// 注释：获取链表的下一条消息
	//
	YCTaskMessage* getNext();

	//
	// 注释：设置NPC消息单元
	//
	void setTalk(YCIMessageUnit* unit);

	//
	// 注释：取得NPC消息单元
	//
	const YCIMessageUnit* getTalk() const;

	//
	// 设置命令消息，所有的命令消息组成一个链表
	// 一个命令解析成一行，可以使图片链接，也可以使文字链接
	//
	void setCommand(YCIMessageUnit* current);

	//
	// 获取命令消息，返回链表的下一个消息单元
	// 如果current为空，返回链表的第一个消息单元
	//
	const YCIMessageUnit* getCommand() const;
		
public:

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
	static void onClickCommand(YCTaskMessage* message, const std::string& action, const std::string& command);

private:

	char mySectionName[TASK_SECTION_NAME];

	YCIMessageUnit* myTalk;

	YCIMessageUnit* myCommands;

	YCTaskMessage* myNext;
};

#endif

