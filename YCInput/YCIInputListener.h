#ifndef _INCLUDE_YCIINPUTLISTENER_H_
#define _INCLUDE_YCIINPUTLISTENER_H_

#include "YCBasic\YCDef.h"

/*
 * YCIInputListener : ������꣬������Ϣ�ӿ�
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
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

};

#endif