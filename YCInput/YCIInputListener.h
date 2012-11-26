#ifndef _INCLUDE_YCIINPUTLISTENER_H_
#define _INCLUDE_YCIINPUTLISTENER_H_

#include "YCBasic\YCDef.h"

/*
 * YCIInputListener : 处理鼠标，键盘消息接口
 */
class YCAPI YCIInputListener
{
public:

	YCIInputListener()
	{
	}

	virtual ~YCIInputListener()
	{
	}

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

};

#endif