#ifndef _YCWMTIMER_H_
#define _YCWMTIMER_H_

#include "YCDef.h"
#include "YCITimer.h"

/*
 * YCWMTimer : ����WM_TIMER��Ϣʵ��
 */
class YCAPI YCWMTimer : public YCITimer
{
public:

	YCWMTimer(void);

	virtual ~YCWMTimer(void);

	//
	// ������bind2HWND(HWND hWnd)
	//
	// Ŀ�ģ��󶨵����ھ��
	//
	void bind2HWND(HWND hWnd);

	//
	// ������start()
	//
	// Ŀ�ģ�������ʱ��
	//
	virtual bool start();

	//
	// ������stop()
	//
	// Ŀ�ģ�������ʱ��
	//
	virtual void stop();

private:

	HWND myHWnd;

	UINT_PTR myEventId;
};

#endif

