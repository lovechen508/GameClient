#ifndef _INCLUDE_YCIMESSAGE_H_
#define _INCLUDE_YCIMESSAGE_H_

#include "YCGraphic/YCIModel.h"

struct YCIUnitAction;

/*
 * YCIMessageUnit : 所有消息单元的抽象 聊天消息，任务消息，弹窗消息。。。
 *                  所有消息单元连接成一个链表，一个链表标示一行
 */
class YCIMessageUnit : YCIModel
{
public:

	YCIMessageUnit(void);

	virtual ~YCIMessageUnit(void);

	//
	// 函数：bindMouseClick(YCIUnitAction* action)
	//
	// 目的：绑定鼠标点击事件
	//
	void bindMouseClick(YCIUnitAction* action);

	//
	// 函数：isMouseClickEnabled()
	//
	// 目的：如果已绑定鼠标点击事件，返回true
	//      否则返回false
	//
	bool isMouseClickEnabled();

	//
	// 函数：onMouseClick()
	//
	// 目的：鼠标点击事件到达
	//
	void onMouseClick();

	//
	// 函数：append(YCIMessageUnit* next)
	//
	// 目的：追加消息单元
	//
	void append(YCIMessageUnit* next);

	//
	// 函数：next() const
	//
	// 目的：后继消息单元
	//
	YCIMessageUnit* next();
	const YCIMessageUnit* next() const;

private:

	//
	// 持有生命周期
	//
	YCIUnitAction* myMouseClickAction;

	//
	// 后继消息单元
	//
	YCIMessageUnit* myNext;
};

#endif

