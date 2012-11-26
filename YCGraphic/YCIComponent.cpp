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

// 快速访问鼠标键盘
YCInput* YCIComponent::gInput = NULL;

// 快速访问绘制精灵对象
YCGraphic::YCD3DSprite * YCIComponent::gSprite = NULL;

// 快速访问基本图形绘制对象
YCGraphic::YCD3DPrimitive * YCIComponent::gPrimitive = NULL;

// 快速访问绘制字符对象
YCGraphic::YCFont * YCIComponent::gFont = NULL;

// 快速访问标签工厂
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
// 函数：modelUpdate(YCEvent& ev)
//
// 目的：通知数据模型更新
//
void YCIComponent::modelUpdate(YCEvent& ev)
{
	// 缺省实现，N/A
}

//
// 函数：setValue(const char* strValue)
//
// 目的：设置元件的值
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
// 函数：getValue()
//
// 目的：获取元件的值
//
const char* YCIComponent::getValue()
{
	return myStrValue;
}

//
// 函数：setOrder(unsigned int order)
//
// 目的：设置当前元件在的UILayer中的order
//
void YCIComponent::setOrder(unsigned int order)
{
	myOrder = order;
}

//
// 函数：getOrder()
//
// 目的：得到当前元件在的UILayer中的order
//       order值越大，距用户越近
//
// 注释：在YCIComponent中无设置order接口
//       所有元件的order默认为0，按照添加到
//       UILayer的先后顺序逆序画
//
unsigned int YCIComponent::getOrder()
{
	return myOrder;
}

//
// 函数：E_COMPONENT_STATUS()
//
// 目的：取得窗口名称
//
YCIComponent::E_COMPONENT_STATUS YCIComponent::getStatus()
{
	return myComponentStatus;
}

//
// 函数：removeListener(E_UITAG_EVENT event, const char* scripts)
//
// 目的：删除消息监听器
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
// 函数：getListener(E_UITAG_EVENT event)
//
// 目的：获取消息监听器
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
// 函数：removeListener(E_UITAG_EVENT event)
//
// 目的：删除消息监听器
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