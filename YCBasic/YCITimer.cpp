#include "YCITimer.h"

#include "YCDList.h"
#include "YCLogger.h"
#include "YCAssert.h"
#include "YCException.h"

struct YCITimerItem
{
	YCITimerItem(unsigned int id, 
				 YCITimer::YCITimerCallback * cb,
				 unsigned int interval)
		: msgId(id)
		, callback(cb)
		, intervalInMillisecond(interval)
		, lastExecuted(0)
		, executedTimes(0)
	{
	}

	unsigned int msgId;
	YCITimer::YCITimerCallback * callback;
	unsigned int intervalInMillisecond;
	unsigned int lastExecuted;
	unsigned int executedTimes;
};

YCITimer::YCITimer()
	: myCallbacks(new YCDList())
	, myIsTriggering(false)
{
}

YCITimer::~YCITimer()
{
	delete myCallbacks;
}

//
// ������registCallback(unsigned int msgId, 
//	                unsigned int intervalInMillisecond, 
//					YCITimerCallback* callback,
//					int times);
//
// Ŀ�ģ�ע�ᶨʱ���ص�
//
// ע�ͣ�msgId��callback ����Ψһȷ����һ��ע��
//      intervalInMillisecond �ص�ʱ����������Ϊ10ms�ı���
//      times �ص��Ĵ���
//
void YCITimer::registCallback(unsigned int msgId, 
		            YCITimerCallback* callback, 
					unsigned int intervalInMillisecond)
{
	const unsigned int SECOND = 1000;

	SMART_ASSERT(!myIsTriggering);
	SMART_ASSERT(callback != NULL);
	SMART_ASSERT(msgId > 0);
	SMART_ASSERT(intervalInMillisecond <= SECOND);

	if (msgId == 0)
	{
		throw YCException(2002, "YCITimer::registCallbackע����ϢIDΪ0");
	}

	if (callback == NULL)
	{
		throw YCException(2002, "YCITimer::registCallbackע����Ϣ�ص�����ΪNULL");
	}

	if (intervalInMillisecond > SECOND)
	{
		throw YCException(2002, "YCITimer::registCallbackע����Ϣ�ص��������1��");
	}

	// ����Ƿ�ID�ظ�
	for (Item_List_T* item = myCallbacks->begin();
		 item != myCallbacks->end();
		 item = myCallbacks->next(item))
	{
		YCITimerItem * timerItem = (YCITimerItem*)myCallbacks->payload(item);
		if (timerItem->msgId == msgId && timerItem->callback == callback)
		{ 
			throw YCException(2002, "YCITimer::registCallbackע����Ϣ�ص��ظ�");
		}
	}

	YCITimerItem* newItem = new YCITimerItem(msgId, callback, intervalInMillisecond);
	myCallbacks->append(newItem, NULL);
}

//
// ������unregistCallback(unsigned int msgId, YCITimerCallback* callback)
//
// Ŀ�ģ��Ƴ���ʱ���ص�
//
void YCITimer::unregistCallback(unsigned int msgId, YCITimerCallback* callback)
{
	SMART_ASSERT(!myIsTriggering);
	SMART_ASSERT(callback != NULL);
	SMART_ASSERT(msgId > 0);

	if (msgId == 0)
	{
		throw YCException(2002, "YCITimer::unregistCallbackע����ϢIDΪ0");
	}

	if (callback == NULL)
	{
		throw YCException(2002, "YCITimer::unregistCallbackע����Ϣ�ص�����ΪNULL");
	}

	for (Item_List_T* item = myCallbacks->begin();
		 item != myCallbacks->end();
		 item = myCallbacks->next(item))
	{
		YCITimerItem * timerItem = (YCITimerItem*)myCallbacks->payload(item);
		if (timerItem->msgId == msgId && timerItem->callback == callback)
		{ 
			myCallbacks->remove(item);
			SAFE_DELETE(timerItem);
			break;
		}
	}

}

//
// ������trigger(unsigned int now)
//
// Ŀ�ģ��̳�ʵ������붨ʱ������20ms������ô˺���
//
void YCITimer::trigger(unsigned int now)
{
	myIsTriggering = true;
	for (Item_List_T* item = myCallbacks->begin();
		 item != myCallbacks->end();
		 item = myCallbacks->next(item))
	{
		YCITimerItem * timerItem = (YCITimerItem*)myCallbacks->payload(item);
		if ( (now - timerItem->lastExecuted) >= timerItem->intervalInMillisecond)
		{
			int msgId = timerItem->msgId;
			timerItem->lastExecuted = now;
			try 
			{
				timerItem->callback->onTimer(msgId, ++timerItem->executedTimes);
			}
			catch (YCException& e)
			{
				LOG_WARNING("YCITimer::trigger ִ��MsgId[" << msgId << "]�����쳣��" << e.what());
			}
		}
	}
	myIsTriggering = false;
}