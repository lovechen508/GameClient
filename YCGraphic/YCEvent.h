#ifndef _INCLUDE_YCEVENT_H_
#define _INCLUDE_YCEVENT_H_

#include "YCBasic\YCAssert.h"

class YCIModel;

/*
 * YCEvent : 由YCIModel向YCIComponent发送的消息类
 */
struct YCEvent
{
	YCEvent(unsigned int id, YCIModel* model, void* info)
		: eventId(id), source(model), addition(info)
	{
		SMART_ASSERT(model != NULL);
	}

	//
	// 消息ID
	//
	unsigned int eventId;

	//
	// 消息源
	//
	YCIModel*	 source;

	//
	// 附加自定义信息，不持有生命周期
	//
	void*		 addition;
};

#endif