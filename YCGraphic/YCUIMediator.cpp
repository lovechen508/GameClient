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
// �����λ
//
void YCUIMediator::resize(unsigned int largerLength)
{
	if (largerLength != myLength)
	{
		if (largerLength < myLength)
		{
			throw YCException(1002, "YCUIMediator::resize��֧�ֲ�λ����");
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
// ����ĳ����λ��Model   M->V
//
void YCUIMediator::setModel(unsigned int slot, YCIModel* model)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::setModel��λ�±��쳣");
	}

	if (model == NULL)
	{
		throw YCException(1002, "YCUIMediator::setModel��λmodelΪNULL");
	}

	if (mySlots[slot]->model != NULL)
	{
		throw YCException(1002, "YCUIMediator::setModel��λԭmodel��ΪNULL");
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
// ��ȡĳ����λ��Model   V->M
//
YCIModel* YCUIMediator::getModel(unsigned int slot)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::getModel��λ�±��쳣");
	}

	return mySlots[slot]->model;
}

//
// �Ƴ�ĳ����λ��Model   M->V
//
void YCUIMediator::removeModel(unsigned int slot)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::removeModel��λ�±��쳣");
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
// ����򿪺���Ӱ� V->M
//
void YCUIMediator::bindSlot(unsigned int slot, YCIComponent* component)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::bindSlot��λ�±��쳣");
	}

	if (component == NULL)
	{
		throw YCException(1002, "YCUIMediator::bindSlot��λcomponentΪNULL");
	}

	mySlots[slot]->monitors->append(component, NULL);
}

//
// ����ر�ʱ����� V->M
//
void YCUIMediator::unbind(YCIComponent* component)
{
	if (component == NULL)
	{
		throw YCException(1002, "YCUIMediator::unbind��λcomponentΪNULL");
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
// ����ĳ����λ��Model   M->V
//
void YCUIMediator::modelUpdated(unsigned int slot)
{
	if (slot >= myLength)
	{
		throw YCException(1002, "YCUIMediator::modelUpdated��λ�±�����");
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
// ����ָ�����Ȳ�λ
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
// �ͷ����в�λ
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

