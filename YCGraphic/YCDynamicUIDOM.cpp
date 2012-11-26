#include "YCDynamicUIDOM.h"

#include "YCUIDOM.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

/////////////////////////////////////////////
// Build Pattern
/////////////////////////////////////////////

//
// 函数：YCDynamicUIDOM* build(YCDynamicUIDOM* uiDom)
//
// 目的：从YCUIDOM中构建内存动态DOM
//
YCDynamicUIDOM* YCDynamicUIDOM::build(const YCUIDOM* uiDom)
{
	YCDynamicUIDOM* built = NULL;

	if (uiDom != NULL)
	{
		built = new YCDynamicUIDOM();
		built->setName(uiDom->getName());
		built->setValue(uiDom->getValue());
		
		// 属性
		const YCDList* attributes = uiDom->getAttributes();
		for (const Item_List_T* item = attributes->begin();
			 item != attributes->end();
			 item = attributes->next(item))
		{
			YCUIAttribute* attribute = (YCUIAttribute*)attributes->payload(item);
			built->setAttribute(attribute->name.c_str(), attribute->value.c_str());
		}		
	}

	return built;
}

YCDynamicUIDOM::YCDynamicUIDOM(void)
	: mySubUIDOM(new YCDList())
	, myAttributes(new YCDList())
	, myParent(NULL)
	, myTagValue(NULL)
	, myOriginalDOM(NULL)
{
	memset(myTagName, 0, TAG_NAME_LENGTH);
}

YCDynamicUIDOM::~YCDynamicUIDOM(void)
{
	mySubUIDOM->clear(YCDListClearHelper<YCDynamicUIDOM>);
	SAFE_DELETE(mySubUIDOM);
	
	myAttributes->clear(YCDListClearHelper<YCUIAttribute>);
	SAFE_DELETE(myAttributes);
	
	SAFE_DELETE(myTagValue);
}

//
// 函数：bind2DOM(const YCUIDOM* uiDom)
//
// 目的：绑定到uiDOM, 不持有生命周期
//
void YCDynamicUIDOM::bind2DOM(const YCUIDOM* uiDom)
{
	myOriginalDOM = uiDom;
}

//
// 函数：getUIDOM()
//
// 目的：取得uiDOM
//
const YCUIDOM* YCDynamicUIDOM::getUIDOM() const
{
	return myOriginalDOM;
}

//
// 函数：setName(const char* name)
//
// 目的：设置DOM节点名
//
void YCDynamicUIDOM::setName(const char* name)
{
	SMART_ASSERT(name != NULL);
	memcpy(myTagName, name, strlen(name));
}

//
// 函数：getName()
//
// 目的：获取DOM节点名
//
const char* YCDynamicUIDOM::getName() const
{
	return myTagName;
}

//
// 函数：setValue(const char* value)
//
// 目的：设置DOM节点值
//
void YCDynamicUIDOM::setValue(const char* value)
{
	if (value == NULL)
	{
		return;
	}

	int len = strlen(value);
	if (len > 0)
	{
		myTagValue = new char[len+1];
		memset(myTagValue, 0, len+1);
		memcpy(myTagValue, value, len);
	}
}

//
// 函数：getValue()
//
// 目的：获取DOM节点值
//
const char* YCDynamicUIDOM::getValue() const
{
	return myTagValue;
}

//
// 函数：getParent()
//
// 目的：获取父DOM节点名
//
YCDynamicUIDOM* YCDynamicUIDOM::getParent()
{
	return myParent;
}

//
// 函数：setParent(YCDynamicUIDOM * parent)
//
// 目的：设置父DOM节点名
//
void YCDynamicUIDOM::setParent(YCDynamicUIDOM * parent)
{
	myParent = parent;
}


//
// 函数：setAttribute(const char* name, const char* value)
//
// 目的：设置属性
//
void YCDynamicUIDOM::setAttribute(const char* name, const char* value)
{
	if (name == NULL)
	{
		throw YCException(2002, "YCDynamicUIDOM::setAttribute属性名为NULL");
	}

	bool found = false;
	for (Item_List_T *item = myAttributes->begin();
		 item != myAttributes->end();
		 item = myAttributes->next(item))
	{
		YCUIAttribute* current = (YCUIAttribute*)myAttributes->payload(item);
		if (current->name == name)
		{
			current->value = value;
			found = true;
			break;
		}
	}

	if (!found)
	{
		YCUIAttribute * attribute = new YCUIAttribute(name, value);
		myAttributes->append(attribute, NULL);
	}
}

//
// 函数：getAttribute(const char* name)
//
// 目的：获取属性值
//
const char* YCDynamicUIDOM::getAttribute(const char* name) const
{
	if (name == NULL)
	{
		throw YCException(2002, "YCDynamicUIDOM::getAttribute属性名为NULL");
	}

	const char* result = NULL;
	for (Item_List_T *item = myAttributes->begin();
		 item != myAttributes->end();
		 item = myAttributes->next(item))
	{
		YCUIAttribute* current = (YCUIAttribute*)myAttributes->payload(item);
		if (current->name == name)
		{
			result = current->value.c_str();
			break;
		}
	}
	return result;
}

//
// 函数：getAttributes()
//
// 目的：获取属性列表
//
const YCDList * YCDynamicUIDOM::getAttributes() const 
{
	return myAttributes;
}

//
// 函数：getSubDom()
//
// 目的：取得子节点集合
//
const YCDList * YCDynamicUIDOM::getSubDom() const
{
	return mySubUIDOM;
}

YCDList * YCDynamicUIDOM::getSubDom()
{
	return mySubUIDOM;
}

//
// 函数：addSubDom(YCDynamicUIDOM* dom)
//
// 目的：添加SubDom子节点
//
void YCDynamicUIDOM::addSubDom(YCDynamicUIDOM* dom)
{
	if (dom != NULL)
	{
		SMART_ASSERT(dom->myParent == NULL);
		mySubUIDOM->append(dom, NULL);
		dom->myParent = this;
	}
}

//
// 函数：removeSubDom(YCDynamicUIDOM* dom)
//
// 目的：删除SubDom子节点
//
YCDynamicUIDOM * YCDynamicUIDOM::removeSubDom(YCDynamicUIDOM* dom)
{
	YCDynamicUIDOM* removed = NULL;
	if (dom != NULL)
	{
		Item_List_T* item = mySubUIDOM->find(dom);
		if (item != NULL)
		{
			removed = (YCDynamicUIDOM*)mySubUIDOM->remove(item);
			removed->myParent = NULL;
		}
	}
	return removed;
}

//
// 函数：updateSubDom(YCDynamicUIDOM* dom)
//
// 目的：替换SubDom子节点
//
bool YCDynamicUIDOM::updateSubDom(YCDynamicUIDOM* old, YCDynamicUIDOM* add)
{
	SMART_ASSERT(old != NULL);
	SMART_ASSERT(add != NULL);

	Item_List_T* item = mySubUIDOM->find(old);
	if (item != NULL)
	{
		mySubUIDOM->append(add, item);
		mySubUIDOM->remove(item);
		return true;
	}
	return false;
}

//
// 函数：removeSubDom(const char* name)
//
// 目的：删除SubDom子节点
//
YCDynamicUIDOM* YCDynamicUIDOM::removeSubDom(const char* name)
{
	throw YCException(2002, "YCDynamicUIDOM::removeSubDom尚未实现");
	return NULL;
}