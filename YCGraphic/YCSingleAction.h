#ifndef _INCLUDE_YCSINGLEACTION_H_
#define _INCLUDE_YCSINGLEACTION_H_

#include "YCIAction.h"

class YCSingleAction :
	public YCIAction
{
public:

	YCSingleAction(void);

	virtual ~YCSingleAction(void);

	//
	// ������run(unsigned int timestamp)
	//
	// Ŀ�ģ��ܶ���
	//
	virtual void run(unsigned int timestamp);
};

#endif

