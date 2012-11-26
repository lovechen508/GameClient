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
// 函数：registCallback(unsigned int msgId, 
//	                unsigned int intervalInMillisecond, 
//					YCITimerCallback* callback,
//					int times);
//
// 目的：注册定时器回调
//
// 注释：msgId和callback 两者唯一确定了一个注册
//      intervalInMillisecond 回调时间间隔，必须为10ms的倍数
//      times 回调的次数
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
		throw YCException(2002, "YCITimer::registCallback注册消息ID为0");
	}

	if (callback == NULL)
	{
		throw YCException(2002, "YCITimer::registCallback注册消息回调函数为NULL");
	}

	if (intervalInMillisecond > SECOND)
	{
		throw YCException(2002, "YCITimer::registCallback注册消息回调间隔大于1秒");
	}

	// 检查是否ID重复
	for (Item_List_T* item = myCallbacks->begin();
		 item != myCallbacks->end();
		 item = myCallbacks->next(item))
	{
		YCITimerItem * timerItem = (YCITimerItem*)myCallbacks->payload(item);
		if (timerItem->msgId == msgId && timerItem->callback == callback)
		{ 
			throw YCException(2002, "YCITimer::registCallback注册消息回调重复");
		}
	}

	YCITimerItem* newItem = new YCITimerItem(msgId, callback, intervalInMillisecond);
	myCallbacks->append(newItem, NULL);
}

//
// 函数：unregistCallback(unsigned int msgId, YCITimerCallback* callback)
//
// 目的：移除定时器回调
//
void YCITimer::unregistCallback(unsigned int msgId, YCITimerCallback* callback)
{
	SMART_ASSERT(!myIsTriggering);
	SMART_ASSERT(callback != NULL);
	SMART_ASSERT(msgId > 0);

	if (msgId == 0)
	{
		throw YCException(2002, "YCITimer::unregistCallback注册消息ID为0");
	}

	if (callback == NULL)
	{
		throw YCException(2002, "YCITimer::unregistCallback注册消息回调函数为NULL");
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
// 函数：trigger(unsigned int now)
//
// 目的：继承实现类必须定时不大于20ms间隔调用此函数
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
				LOG_WARNING("YCITimer::trigger 执行MsgId[" << msgId << "]出现异常：" << e.what());
			}
		}
	}
	myIsTriggering = false;
}