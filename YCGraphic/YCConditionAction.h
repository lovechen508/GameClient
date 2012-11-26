#ifndef _INCLUDE_YCCONDITIONACTION_H_
#define _INCLUDE_YCCONDITIONACTION_H_

#include "YCIAction.h"

class YCDList;

/*
 * YCConditionAction : ��������������
 *
 *
 *
 */
class YCConditionAction :
	public YCIAction
{
public:

	class YCIActionFunctor
	{
	public:

		YCIActionFunctor() {}

		virtual ~YCIActionFunctor() {}

		//
		// ������judge(unsigned int condition)
		//
		// Ŀ�ģ�ȡ���¸�����
		//
		virtual YCIAction* judge(unsigned int condition) = 0;

	private:

		YCConditionAction* myCondition;
	};

public:

	YCConditionAction(YCIActionFunctor* condition);

	virtual ~YCConditionAction(void);

	//
	// ������addBranch(YCIAction* action)
	//
	// Ŀ�ģ���ӷ�֧
	//
	void addBranch(YCIAction* action);
	

private:

	YCIActionFunctor * myCondition;

	YCDList * myConditionActions;

};

#endif

