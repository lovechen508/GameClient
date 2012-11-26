#ifndef _YCWMTIMER_H_
#define _YCWMTIMER_H_

#include "YCDef.h"
#include "YCITimer.h"

/*
 * YCWMTimer : 借助WM_TIMER消息实现
 */
class YCAPI YCWMTimer : public YCITimer
{
public:

	YCWMTimer(void);

	virtual ~YCWMTimer(void);

	//
	// 函数：bind2HWND(HWND hWnd)
	//
	// 目的：绑定到窗口句柄
	//
	void bind2HWND(HWND hWnd);

	//
	// 函数：start()
	//
	// 目的：启动定时器
	//
	virtual bool start();

	//
	// 函数：stop()
	//
	// 目的：启动定时器
	//
	virtual void stop();

private:

	HWND myHWnd;

	UINT_PTR myEventId;
};

#endif

