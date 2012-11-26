#ifndef _INCLUDE_YCCHATMESSAGE_H_
#define _INCLUDE_YCCHATMESSAGE_H_

class YCIMessageUnit;

/*
 * YCChatMessage : 单条消息具现类
 */
class YCChatMessage
{
public:

	//
	// 构造函数
	//
	YCChatMessage(int type, const char* name);

	~YCChatMessage(void);

	//
	// 函数：setUnit(YCIMessageUnit* unit)
	//
	// 目的：设置消息单元，接管生命周期
	//
	void setUnit(YCIMessageUnit* unit);

	//
	// 函数：getUnit()
	//
	// 目的： 取得消息单元
	//
	YCIMessageUnit* getUnit();

private:

	//
	// 消息类型 CH_LIGHT ...
	//
	int myType;

	//
	// 发送角色名
	//
	char* mySender;

	//
	// 消息单元链表
	//
	YCIMessageUnit* myUnits;
};

#endif