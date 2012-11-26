#ifndef _INCLUDE_YCUIPARALLELACTION_H_
#define _INCLUDE_YCUIPARALLELACTION_H_

#include "YCIUIAction.h"

class YCDList;

/*
 * YCUIParallelAction : ����ִ�е�Action
 *
 * ˵����
 *
 */
class YCUIParallelAction : YCIUIAction
{
public:

	YCUIParallelAction(int durationTimes);

	virtual ~YCUIParallelAction();
	
	//
	// ������addAction(YCIUIAction* action)
	//
	// Ŀ�ģ����Action�����ж���
	//
	void addAction(YCIUIAction* action);

	//
	// ������bool run()
	//
	// Ŀ�ģ���YCTimer��������run����
	//
	// ע�ͣ�
	//       ����������front��ActionQueue�еõ������
	//       ����ִ����ɷ���true, ���ActionQueue��pop����
	//       ����ִ����ɷ���false, ����Ȼ������ActionQueue��
	//
	virtual bool run();

private:
	
	YCDList * myActions;
};

#endif