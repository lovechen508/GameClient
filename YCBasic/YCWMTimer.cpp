#include "YCWMTimer.h"

#include "YCLogger.h"
#include "YCException.h"

YCWMTimer::YCWMTimer(void)
{
}


YCWMTimer::~YCWMTimer(void)
{
}

//
// 函数：bind2HWND(HWND hWnd)
//
// 目的：绑定到窗口句柄
//
void YCWMTimer::bind2HWND(HWND hWnd)
{
	myHWnd = hWnd;
}

//
// 函数：start()
//
// 目的：启动定时器
//
bool YCWMTimer::start()
{
	myEventId = ::SetTimer(myHWnd, NULL, 10, NULL);
	if (myEventId == 0)
	{
		throw YCException(2002, "YCWMTimer::start定时器启动失败!");
	}
	return true;
}

//
// 函数：stop()
//
// 目的：启动定时器
//
void YCWMTimer::stop()
{
	if (!::KillTimer(myHWnd, myEventId))
	{
		LOG_INFO("YCWMTimer::stop定时器停止失败:" << ::GetLastError());
		//throw YCException(2002, "YCWMTimer::start定时器停止失败!");
	}
}
