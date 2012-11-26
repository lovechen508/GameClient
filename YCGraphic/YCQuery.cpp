#include "YCQuery.h"

#include <regex>

#include "YCIUITag.h"
#include "YCUITagDef.h"

#include "YCLua\YCLuaContext.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

#define RETRIEVE_AND_VALIDATE_SELECTOR YCDList* selector = (YCDList*)myContext->lookup("selector"); \
	                                   if (selector == NULL) \
									   { \
											throw YCException(2002, "YCQuery��������ǰδ�ܻ��ѡȡ�����Ƿ����˵���selector��"); \
									   }

//
// ������YCQuery()
//
// Ŀ�ģ����캯�����󶨵�����
//
YCQuery::YCQuery()
{
	
}

YCQuery::~YCQuery()
{
}

//
// ������bind(YCLuaContext * context)
//
// Ŀ�ģ���������
//
void YCQuery::bind(YCLuaContext * context)
{
	SMART_ASSERT(context != NULL);
	myContext = context;
}


//
// ������$(const char* selector)
//
// Ŀ�ģ�����selectorѡ����ȡ��YCIUITag�б�
//
YCQuery& YCQuery::$(const char* selector)
{
	YCDList* tags = new YCDList();
	if (selector == NULL) 
	{
		void* container = myContext->lookup("container");
		if (container == NULL)
		{
			throw YCException(2002, "YCQuery::$��������û������container��");
		}
		tags->append(container, NULL);
	}
	else if (strcmp(selector, "this") == 0)
	{
		void* current = myContext->lookup("this");
		if (current == NULL)
		{
			throw YCException(2002, "YCQuery::$��������û������this��ʶ��ǰtag��");
		}
		tags->append(current, NULL);
	}
	else 
	{
		void* container = myContext->lookup("container");
		if (container == NULL)
		{
			throw YCException(2002, "YCQuery::$��������û������container��");
		}
	}

	myContext->updateSelector(tags);

	return *this;
}

//
// ������css(const char *attr, const char *value)
//
// Ŀ�ģ��޸�CSS����
//
YCQuery& YCQuery::css(const char *css, const char *value)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������attr(const char* attr, const char *value)
//
// Ŀ�ģ�
//
YCQuery& YCQuery::attr(const char* attr, const char *value)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������value(const char* value)
//
// Ŀ�ģ�����Ԫ����ֵ
//
YCIQuery& YCQuery::value(const char* value)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	for (Item_List_T* item = selector->begin();
		 item != selector->end();
		 item = selector->next(item))
	{
		YCIUITag* tag = (YCIUITag*)selector->payload(item);
		tag->setValue(value);
	}

	return *this;
}

//
// ������value()
//
// Ŀ�ģ���ȡԪ����ֵ
//
const char* YCQuery::value()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;
	if (selector->size() == 1)
	{
		YCIUITag* tag = (YCIUITag*)selector->payload(selector->begin());
		return tag->getValue();
	}
	return NULL;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::html(const char* html)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	for (Item_List_T* item = selector->begin();
		 item != selector->end();
		 item = selector->next(item))
	{
		YCIUITag* tag = (YCIUITag*)selector->payload(item);
		tag->html(html);
	}
	return *this;
}

//
// ������append()
//
// Ŀ�ģ�
//
YCQuery& YCQuery::append(const char* html)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;
	
	for (Item_List_T* item = selector->begin();
		 item != selector->end();
		 item = selector->next(item))
	{
		YCIUITag* tag = (YCIUITag*)selector->payload(item);
		tag->append(html);
	}

	return *this;
}

//
// ������each()
//
// Ŀ�ģ�
//
YCQuery& YCQuery::each()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;
	return *this;
}

//
// ������hover()
//
// Ŀ�ģ�
//
YCQuery& YCQuery::hover()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������toggle()
//
// Ŀ�ģ�
//
YCQuery& YCQuery::toggle()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������hide()
//
// Ŀ�ģ�
//
YCQuery& YCQuery::hide()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������show()
//
// Ŀ�ģ�
//
YCQuery& YCQuery::show()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������fadeIn()
//
// Ŀ�ģ�
//
YCQuery& YCQuery::fadeIn()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������fadeOut()
//
// Ŀ�ģ�
//
YCQuery& YCQuery::fadeOut()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::animation()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������move(const char* speed, int x, int y)
//
// Ŀ�ģ��ƶ�Ԫ�����ĵ㵽(x, y)
//
YCQuery& YCQuery::move(const char* speed, int x, int y)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;
/*
	int noOfTags = selector->size();
	if (noOfTags != 1)
	{
		LOG_WARNING("YCQuery::moveѡ����������Ψһ��" << noOfTags);
	}

	Item_List_T* item = selector->begin();
	YCIUITag* tag = (YCIUITag*)selector->payload(item);
	if (strcmp("slow", speed) == 0)
	{
		tag->move(UITAG_MOVE_SLOW, x, y);
	}
	else if (strcmp("normal", speed) == 0)
	{
		tag->move(UITAG_MOVE_NORMAL, x, y);
	}
	else if (strcmp("fast", speed) == 0)
	{
		tag->move(UITAG_MOVE_FAST, x, y);
	}
	*/
	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::parent()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::child()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::click()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::submit()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::slideUp()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::slideDown()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::addClass()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������
//
// Ŀ�ģ�
//
YCQuery& YCQuery::removeClass()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// ������end()
//
// Ŀ�ģ����selector
//
YCQuery& YCQuery::end()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	myContext->updateSelector(NULL);

	return *this;
}
