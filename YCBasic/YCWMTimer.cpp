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
// ������bind2HWND(HWND hWnd)
//
// Ŀ�ģ��󶨵����ھ��
//
void YCWMTimer::bind2HWND(HWND hWnd)
{
	myHWnd = hWnd;
}

//
// ������start()
//
// Ŀ�ģ�������ʱ��
//
bool YCWMTimer::start()
{
	myEventId = ::SetTimer(myHWnd, NULL, 10, NULL);
	if (myEventId == 0)
	{
		throw YCException(2002, "YCWMTimer::start��ʱ������ʧ��!");
	}
	return true;
}

//
// ������stop()
//
// Ŀ�ģ�������ʱ��
//
void YCWMTimer::stop()
{
	if (!::KillTimer(myHWnd, myEventId))
	{
		LOG_INFO("YCWMTimer::stop��ʱ��ֹͣʧ��:" << ::GetLastError());
		//throw YCException(2002, "YCWMTimer::start��ʱ��ֹͣʧ��!");
	}
}
