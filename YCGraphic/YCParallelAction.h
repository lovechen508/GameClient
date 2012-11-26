#ifndef _INCLUDE_YCPARALLELACTION_H_
#define _INCLUDE_YCPARALLELACTION_H_

#include "YCIAction.h"

class YCDList;

/*
 * YCParallelAction : Sprite���ж�����
 *
 *
 *
 *
 *
 */
class YCParallelAction :
	public YCIAction
{
public:

	//
	// ������YCParallelAction(YCIAction* baseAction)
	//
	// Ŀ�ģ��ṩ�����������������ж���
	//
	YCParallelAction(YCIAction* baseAction);

	virtual ~YCParallelAction(void);

	//
	// ������run(unsigned int timestamp)
	//
	// Ŀ�ģ��ܶ���
	//
	void run(unsigned int timestamp);

	//
	// ������addAction(YCIAction* action)
	//
	// Ŀ�ģ����벢�ж���
	//
	void addAction(YCIAction* action);

	//
	// ������removeAction(YCIAction* action)
	//
	// Ŀ�ģ��Ƴ����ж���
	//
	void removeAction(YCIAction* action);

private:

	//
	// ���ԣ�myParallelActions
	//
	// Ŀ�ģ����ж����б�
	//      �����Ⱥ�˳����Ⱦ
	//
	YCDList *myParallelActions;

};

#endif
