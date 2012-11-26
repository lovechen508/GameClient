#include "YCUIMediator.h"

#include "YCIModel.h"
#include "YCIComponent.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCUIMediator::YCUIMediator(unsigned int length)
	: myLength(length)
	, mySlots(NULL)
{
	SMART_ASSERT(myLength > 0);

	mySlots = createSlots(length);
}

YCUIMediator::~YCUIMediator(void)
{
	releaseSlots();
}

//
// 增大槽位
//
void YCUIMediator::resize(unsigned int largerLength)
{
	if (largerLength != myLength)
	{
		if (largerLength < myLength)
		{
			throw YCException(1002, "YCUIMediator::resize不支持槽位收缩");
		}

		Slot_T** tmp = createSlots(largerLength);
		for (unsigned int i = 0; i < myLength; ++i)
		{
			std::swap(tmp[i], mySlots[i]);
		}
		releaseSlots();

		mySlots = tmp;
		myLength = largerLength;
	}
}

//
// 设置某个槽位的Model   M->V
//
void YCUIMediator::setModel(unsigned int slot, YCIModel* model)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::setModel槽位下标异常");
	}

	if (model == NULL)
	{
		throw YCException(1002, "YCUIMediator::setModel槽位model为NULL");
	}

	if (mySlots[slot]->model != NULL)
	{
		throw YCException(1002, "YCUIMediator::setModel槽位原model不为NULL");
	}

	mySlots[slot]->model = model;
	
	YCDList* theMonitors = mySlots[slot]->monitors;
	for (Item_List_T* item = theMonitors->begin();
		 item != theMonitors->end();
		 item == theMonitors->next(item))
	{
		YCIComponent* component = (YCIComponent*)theMonitors->payload(item);
		//component->modelUpdate()
		//component->update(model);
	}
}

//
// 获取某个槽位的Model   V->M
//
YCIModel* YCUIMediator::getModel(unsigned int slot)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::getModel槽位下标异常");
	}

	return mySlots[slot]->model;
}

//
// 移除某个槽位的Model   M->V
//
void YCUIMediator::removeModel(unsigned int slot)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::removeModel槽位下标异常");
	}

	if (mySlots[slot]->model != NULL)
	{
		YCDList* theMonitors = mySlots[slot]->monitors;
		for (Item_List_T* item = theMonitors->begin();
			item != theMonitors->end();
			item == theMonitors->next(item))
		{
			YCIComponent* component = (YCIComponent*)theMonitors->payload(item);
			//component->modelUpdate()
			//component->update(NULL);
		}

		SAFE_DELETE(mySlots[slot]->model);
	}
}

//
// 界面打开后添加绑定 V->M
//
void YCUIMediator::bindSlot(unsigned int slot, YCIComponent* component)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::bindSlot槽位下标异常");
	}

	if (component == NULL)
	{
		throw YCException(1002, "YCUIMediator::bindSlot槽位component为NULL");
	}

	mySlots[slot]->monitors->append(component, NULL);
}

//
// 界面关闭时解除绑定 V->M
//
void YCUIMediator::unbind(YCIComponent* component)
{
	if (component == NULL)
	{
		throw YCException(1002, "YCUIMediator::unbind槽位component为NULL");
	}

	for (unsigned int i = 0; i < myLength; ++i)
	{
		YCDList* monitors = mySlots[i]->monitors;
		for (Item_List_T* item = monitors->begin();
			 item != monitors->end();
			 item == monitors->payload(item))
		{
			YCIComponent* binded = (YCIComponent*)monitors->payload(item);
			if (binded == component)
			{
				monitors->remove(item);
				break;
			}
		}
	}
}

//
// 更新某个槽位的Model   M->V
//
void YCUIMediator::modelUpdated(unsigned int slot)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::modelUpdated槽位下标有误");
	}

	YCDList* theMonitors = mySlots[slot]->monitors;
	for (Item_List_T* item = theMonitors->begin();
		item != theMonitors->end();
		item == theMonitors->next(item))
	{
		YCIComponent* component = (YCIComponent*)theMonitors->payload(item);
		//component->modelUpdate()
		//component->update(mySlots[slot]->model);
	}
}

//////////////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////////////

//
// 创建指定长度槽位
//
YCUIMediator::Slot_T** YCUIMediator::createSlots(unsigned int size)
{
	Slot_T** result = new Slot_T*[size];
	memset(result, 0, sizeof(Slot_T*)*size);

	for (unsigned int i = 0; i < size; ++i)
	{
		result[i] = new Slot_T();
		result[i]->model = NULL;
		result[i]->monitors = new YCDList();
	}

	return result;
}

//
// 释放所有槽位
//
void YCUIMediator::releaseSlots()
{
	for (int i = 0; i < myLength; ++i)
	{
		Slot_T* slot = mySlots[i];
		SAFE_DELETE(slot->model);
		slot->monitors->clear();
		SAFE_DELETE(slot->monitors);

		SAFE_DELETE(mySlots[i]);
	}

	SAFE_DELETE_ARRAY(mySlots);
}

