#include "YCIComponent.h"

#include <string>

#include "YCTagFactory.h"

#include "YCLua\YCLuaParameter.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"

struct YCListener_T {
	E_UITAG_EVENT event;
	std::string scripts;
};

static void component_listener_release(void* item)
{
	YCListener_T* listener = (YCListener_T*)item;
	SAFE_DELETE(item);
}

// ���ٷ���������
YCInput* YCIComponent::gInput = NULL;

// ���ٷ��ʻ��ƾ������
YCGraphic::YCD3DSprite * YCIComponent::gSprite = NULL;

// ���ٷ��ʻ���ͼ�λ��ƶ���
YCGraphic::YCD3DPrimitive * YCIComponent::gPrimitive = NULL;

// ���ٷ��ʻ����ַ�����
YCGraphic::YCFont * YCIComponent::gFont = NULL;

// ���ٷ��ʱ�ǩ����
YCTagFactory* YCIComponent::gTagFactory = NULL;

YCIComponent::YCIComponent(void)
	: myListeners(new YCDList())
	, myComponentStatus(STATUS_CLOSE)
	, myStrValue(NULL)
	, myOrder(0)
{
}

YCIComponent::~YCIComponent(void)
{
	SAFE_DELETE_ARRAY(myStrValue);

	if (myListeners != NULL)
	{
		myListeners->clear(component_listener_release);
		SAFE_DELETE(myListeners);
	}
}

//
// ������modelUpdate(YCEvent& ev)
//
// Ŀ�ģ�֪ͨ����ģ�͸���
//
void YCIComponent::modelUpdate(YCEvent& ev)
{
	// ȱʡʵ�֣�N/A
}

//
// ������setValue(const char* strValue)
//
// Ŀ�ģ�����Ԫ����ֵ
//
void YCIComponent::setValue(const char* strValue)
{
	SAFE_DELETE_ARRAY(myStrValue);

	if (strValue != NULL)
	{
		int len = strlen(strValue);
		myStrValue = new char[len+1];
		memcpy(myStrValue, strValue, len);
		myStrValue[len] = '\0';
	}
}

//
// ������getValue()
//
// Ŀ�ģ���ȡԪ����ֵ
//
const char* YCIComponent::getValue()
{
	return myStrValue;
}

//
// ������setOrder(unsigned int order)
//
// Ŀ�ģ����õ�ǰԪ���ڵ�UILayer�е�order
//
void YCIComponent::setOrder(unsigned int order)
{
	myOrder = order;
}

//
// ������getOrder()
//
// Ŀ�ģ��õ���ǰԪ���ڵ�UILayer�е�order
//       orderֵԽ�󣬾��û�Խ��
//
// ע�ͣ���YCIComponent��������order�ӿ�
//       ����Ԫ����orderĬ��Ϊ0��������ӵ�
//       UILayer���Ⱥ�˳������
//
unsigned int YCIComponent::getOrder()
{
	return myOrder;
}

//
// ������E_COMPONENT_STATUS()
//
// Ŀ�ģ�ȡ�ô�������
//
YCIComponent::E_COMPONENT_STATUS YCIComponent::getStatus()
{
	return myComponentStatus;
}

//
// ������removeListener(E_UITAG_EVENT event, const char* scripts)
//
// Ŀ�ģ�ɾ����Ϣ������
//
void YCIComponent::addListener(E_UITAG_EVENT event, const char* scripts)
{
	SMART_ASSERT(scripts != NULL);

	for (Item_List_T* item = myListeners->begin();
		 item != myListeners->end();
		 item = myListeners->next(item))
	{
		YCListener_T *current = (YCListener_T*)myListeners->payload(item);
		if (current->event == event)
		{
			current->scripts = scripts;
			return;
		}
	}

	YCListener_T* listener = new YCListener_T();
	listener->event = event;
	listener->scripts = scripts;
	myListeners->append(listener, NULL);
}

//
// ������getListener(E_UITAG_EVENT event)
//
// Ŀ�ģ���ȡ��Ϣ������
//
const char* YCIComponent::getListener(E_UITAG_EVENT event)
{
	for (Item_List_T* item = myListeners->begin();
		 item != myListeners->end();
		 item = myListeners->next(item))
	{
		YCListener_T *current = (YCListener_T*)myListeners->payload(item);
		if (current->event == event)
		{
			return current->scripts.c_str();
		}
	}
	return NULL;
}

//
// ������removeListener(E_UITAG_EVENT event)
//
// Ŀ�ģ�ɾ����Ϣ������
//
bool YCIComponent::removeListener(E_UITAG_EVENT event)
{
	bool found = false;
	for (Item_List_T* item = myListeners->begin();
		 item != myListeners->end() && !found;
		 item = myListeners->next(item))
	{
		YCListener_T *current = (YCListener_T*)myListeners->payload(item);
		if (current->event == event)
		{
			myListeners->remove(item);
			SAFE_DELETE(current);
			found = true;
		}
	}
	return found;
}