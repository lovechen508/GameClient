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
// 函数：render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
//
// 目的：各层的渲染接口
//
// 注释：
//      因为zOrder越大越靠前，且myContainers按照zOrder从小到大排序
//      所以从前到后依次渲染就可以了
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
// 函数：addComponent(YCIComponent* component)
//
// 目的：添加元件到UI界面层中, zOrder越大越靠后
//
void YCUILayer::addComponent(YCIComponent* component)
{
	SMART_ASSERT(component != NULL);
	if (myComponents->find(component) != NULL)
	{
		throw YCException(2002, "YCUILayer::addComponent重复添加元件到UI层");
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
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数, zOrder越大的越先得到处理机会，
//      所以反向遍历myContainers
//
// 返回值：已处理，返回true，否则返回false
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
			return true; //已处理
		}
	}
	return false;
}
