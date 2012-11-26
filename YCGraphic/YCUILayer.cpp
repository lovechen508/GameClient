#include "YCUILayer.h"

#include "YCIComponent.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCUILayer::YCUILayer(void)
	: YCISceneLayer(UI)
	, myComponents(new YCDList())
{
}

static void layer_component_release(void* item)
{
	YCIComponent* component = (YCIComponent*)item;
	SAFE_DELETE(component);
}

YCUILayer::~YCUILayer(void)
{
	myComponents->clear(layer_component_release);
	SAFE_DELETE(myComponents);
}

//
// ������render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
//
// Ŀ�ģ��������Ⱦ�ӿ�
//
// ע�ͣ�
//      ��ΪzOrderԽ��Խ��ǰ����myContainers����zOrder��С��������
//      ���Դ�ǰ����������Ⱦ�Ϳ�����
//
void YCUILayer::render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
{
	Item_List_T* item = myComponents->r_begin();
	while (item != myComponents->r_end())
	{
		Item_List_T* next = myComponents->r_next(item);

		YCIComponent *component = (YCIComponent*)myComponents->payload(item);
		if (component->getStatus() == YCIComponent::STATUS_CLOSE)
		{
			myComponents->remove(item);
			SAFE_DELETE(component);
		}
		else
		{
			component->render();
		}

		item = next;
	}

	//for (Item_List_T* item = myContainers->r_begin();
	//	 item != myContainers->r_end();
	//	 item = myContainers->r_next(item))
	//{
	//	YCIComponent *component = (YCIComponent*)myContainers->payload(item);
	//	component->draw();
	//}
}

//
// ������addComponent(YCIComponent* component)
//
// Ŀ�ģ����Ԫ����UI�������, zOrderԽ��Խ����
//
void YCUILayer::addComponent(YCIComponent* component)
{
	SMART_ASSERT(component != NULL);
	if (myComponents->find(component) != NULL)
	{
		throw YCException(2002, "YCUILayer::addComponent�ظ����Ԫ����UI��");
	}

	Item_List_T* position = NULL;
	unsigned int order = component->getOrder();
	for (Item_List_T* item = myComponents->begin();
		 item != myComponents->end() && !position;
		 item = myComponents->next(item))
	{
		YCIComponent* current = (YCIComponent*)myComponents->payload(item);
		if (current->getOrder() > order)
		{
			position = item;
		}
	}

	myComponents->insert(component, position);
}

//
// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������, zOrderԽ���Խ�ȵõ�������ᣬ
//      ���Է������myContainers
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCUILayer::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	for (Item_List_T* item = myComponents->r_begin();
		 item != myComponents->r_end();
		 item = myComponents->r_next(item))
	{
		YCIComponent *component = (YCIComponent*)myComponents->payload(item);
		if (message == WM_KEYDOWN)
		{
			if (component->matchAccelerateKey())
			{
				component->close();
				return true;
			} 
		}

		if (component->onMessage(message, wParam, lParam))
		{
			return true; //�Ѵ���
		}
	}
	return false;
}
