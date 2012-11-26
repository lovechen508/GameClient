#ifndef _INCLUDE_YCCONDITIONACTION_H_
#define _INCLUDE_YCCONDITIONACTION_H_

#include "YCIAction.h"

class YCDList;

/*
 * YCConditionAction : 条件动作分配器
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
		// 函数：judge(unsigned int condition)
		//
		// 目的：取得下个动作
		//
		virtual YCIAction* judge(unsigned int condition) = 0;

	private:

		YCConditionAction* myCondition;
	};

public:

	YCConditionAction(YCIActionFunctor* condition);

	virtual ~YCConditionAction(void);

	//
	// 函数：addBranch(YCIAction* action)
	//
	// 目的：添加分支
	//
	void addBranch(YCIAction* action);
	

private:

	YCIActionFunctor * myCondition;

	YCDList * myConditionActions;

};

#endif

