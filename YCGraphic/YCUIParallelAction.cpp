#include "YCUIParallelAction.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

YCUIParallelAction::YCUIParallelAction(int durationTimes)
	: YCIUIAction(durationTimes)
	, myActions(new YCDList())
{
}

YCUIParallelAction::~YCUIParallelAction()
{
	delete myActions;
}
	
//
// ������addAction(YCIUIAction* action)
//
// Ŀ�ģ����Action�����ж���
//
void YCUIParallelAction::addAction(YCIUIAction* action)
{
	if (action == NULL)
	{
		throw YCException(2002, "YCUIParallelAction::addAction���actionΪNULL");
	}
	myActions->append(action, NULL);
}

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
bool YCUIParallelAction::run()
{
	myDurationTimes -= 1;

	for (Item_List_T* item = myActions->begin();
		 item != myActions->end();
		 item = myActions->next(item))
	{
		YCIUIAction* action = (YCIUIAction*)myActions->payload(item);
		bool finished = action->run();
		if ( (myDurationTimes == 0) && !finished)
		{
			LOG_WARNING("YCUIParallelAction::run�Ӷ����ٶȲ�һ��!");
		}
	}

	return (myDurationTimes == 0);
}