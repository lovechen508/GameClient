#ifndef _INCLUDE_YCEVENT_H_
#define _INCLUDE_YCEVENT_H_

#include "YCBasic\YCAssert.h"

class YCIModel;

/*
 * YCEvent : ��YCIModel��YCIComponent���͵���Ϣ��
 */
struct YCEvent
{
	YCEvent(unsigned int id, YCIModel* model, void* info)
		: eventId(id), source(model), addition(info)
	{
		SMART_ASSERT(model != NULL);
	}

	//
	// ��ϢID
	//
	unsigned int eventId;

	//
	// ��ϢԴ
	//
	YCIModel*	 source;

	//
	// �����Զ�����Ϣ����������������
	//
	void*		 addition;
};

#endif