#ifndef _INCLUDE_YCSEQUENCEACTION_H_
#define _INCLUDE_YCSEQUENCEACTION_H_

#include "YCIAction.h"

/*
 * YCSequenceAction : ˳�����Ͷ���
 *
 *
 */
class YCSequenceAction :
	public YCIAction
{
public:

	YCSequenceAction(void);

	virtual ~YCSequenceAction(void);

	//
	// ������run(unsigned int timestamp)
	//
	// Ŀ�ģ��ܶ���
	//
	virtual void run(unsigned int timestamp);
};

#endif
