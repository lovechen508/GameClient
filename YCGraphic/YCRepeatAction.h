#ifndef _INCLUDE_YCREPEATACTION_H_
#define _INCLUDE_YCREPEATACTION_H_

#include "YCIAction.h"

/*
 * YCRepeatAction : �ظ�ѭ���ද��, ֱ�����ⲿ�����ж�
 *
 *
 */
class YCRepeatAction :	public YCIAction
{
public:

	YCRepeatAction(void);
	
	virtual ~YCRepeatAction(void);

	//
	// ������run(unsigned int timestamp)
	//
	// Ŀ�ģ��ܶ���
	//
	virtual void run(unsigned int timestamp);

};

#endif

