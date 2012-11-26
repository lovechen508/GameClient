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
// ������YCDynamicUIDOM* build(YCDynamicUIDOM* uiDom)
//
// Ŀ�ģ���YCUIDOM�й����ڴ涯̬DOM
//
YCDynamicUIDOM* YCDynamicUIDOM::build(const YCUIDOM* uiDom)
{
	YCDynamicUIDOM* built = NULL;

	if (uiDom != NULL)
	{
		built = new YCDynamicUIDOM();
		built->setName(uiDom->getName());
		built->setValue(uiDom->getValue());
		
		// ����
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
// ������bind2DOM(const YCUIDOM* uiDom)
//
// Ŀ�ģ��󶨵�uiDOM, ��������������
//
void YCDynamicUIDOM::bind2DOM(const YCUIDOM* uiDom)
{
	myOriginalDOM = uiDom;
}

//
// ������getUIDOM()
//
// Ŀ�ģ�ȡ��uiDOM
//
const YCUIDOM* YCDynamicUIDOM::getUIDOM() const
{
	return myOriginalDOM;
}

//
// ������setName(const char* name)
//
// Ŀ�ģ�����DOM�ڵ���
//
void YCDynamicUIDOM::setName(const char* name)
{
	SMART_ASSERT(name != NULL);
	memcpy(myTagName, name, strlen(name));
}

//
// ������getName()
//
// Ŀ�ģ���ȡDOM�ڵ���
//
const char* YCDynamicUIDOM::getName() const
{
	return myTagName;
}

//
// ������setValue(const char* value)
//
// Ŀ�ģ�����DOM�ڵ�ֵ
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
// ������getValue()
//
// Ŀ�ģ���ȡDOM�ڵ�ֵ
//
const char* YCDynamicUIDOM::getValue() const
{
	return myTagValue;
}

//
// ������getParent()
//
// Ŀ�ģ���ȡ��DOM�ڵ���
//
YCDynamicUIDOM* YCDynamicUIDOM::getParent()
{
	return myParent;
}

//
// ������setParent(YCDynamicUIDOM * parent)
//
// Ŀ�ģ����ø�DOM�ڵ���
//
void YCDynamicUIDOM::setParent(YCDynamicUIDOM * parent)
{
	myParent = parent;
}


//
// ������setAttribute(const char* name, const char* value)
//
// Ŀ�ģ���������
//
void YCDynamicUIDOM::setAttribute(const char* name, const char* value)
{
	if (name == NULL)
	{
		throw YCException(2002, "YCDynamicUIDOM::setAttribute������ΪNULL");
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
// ������getAttribute(const char* name)
//
// Ŀ�ģ���ȡ����ֵ
//
const char* YCDynamicUIDOM::getAttribute(const char* name) const
{
	if (name == NULL)
	{
		throw YCException(2002, "YCDynamicUIDOM::getAttribute������ΪNULL");
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
// ������getAttributes()
//
// Ŀ�ģ���ȡ�����б�
//
const YCDList * YCDynamicUIDOM::getAttributes() const 
{
	return myAttributes;
}

//
// ������getSubDom()
//
// Ŀ�ģ�ȡ���ӽڵ㼯��
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
// ������addSubDom(YCDynamicUIDOM* dom)
//
// Ŀ�ģ����SubDom�ӽڵ�
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
// ������removeSubDom(YCDynamicUIDOM* dom)
//
// Ŀ�ģ�ɾ��SubDom�ӽڵ�
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
// ������updateSubDom(YCDynamicUIDOM* dom)
//
// Ŀ�ģ��滻SubDom�ӽڵ�
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
// ������removeSubDom(const char* name)
//
// Ŀ�ģ�ɾ��SubDom�ӽڵ�
//
YCDynamicUIDOM* YCDynamicUIDOM::removeSubDom(const char* name)
{
	throw YCException(2002, "YCDynamicUIDOM::removeSubDom��δʵ��");
	return NULL;
}