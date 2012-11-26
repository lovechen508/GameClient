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
											throw YCException(2002, "YCQuery函数调用前未能获得选取器，是否忘了调用selector？"); \
									   }

//
// 函数：YCQuery()
//
// 目的：构造函数，绑定到窗口
//
YCQuery::YCQuery()
{
	
}

YCQuery::~YCQuery()
{
}

//
// 函数：bind(YCLuaContext * context)
//
// 目的：绑定上下文
//
void YCQuery::bind(YCLuaContext * context)
{
	SMART_ASSERT(context != NULL);
	myContext = context;
}


//
// 函数：$(const char* selector)
//
// 目的：根据selector选择器取得YCIUITag列表
//
YCQuery& YCQuery::$(const char* selector)
{
	YCDList* tags = new YCDList();
	if (selector == NULL) 
	{
		void* container = myContext->lookup("container");
		if (container == NULL)
		{
			throw YCException(2002, "YCQuery::$上下文中没有设置container！");
		}
		tags->append(container, NULL);
	}
	else if (strcmp(selector, "this") == 0)
	{
		void* current = myContext->lookup("this");
		if (current == NULL)
		{
			throw YCException(2002, "YCQuery::$上下文中没有设置this标识当前tag！");
		}
		tags->append(current, NULL);
	}
	else 
	{
		void* container = myContext->lookup("container");
		if (container == NULL)
		{
			throw YCException(2002, "YCQuery::$上下文中没有设置container！");
		}
	}

	myContext->updateSelector(tags);

	return *this;
}

//
// 函数：css(const char *attr, const char *value)
//
// 目的：修改CSS属性
//
YCQuery& YCQuery::css(const char *css, const char *value)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：attr(const char* attr, const char *value)
//
// 目的：
//
YCQuery& YCQuery::attr(const char* attr, const char *value)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：value(const char* value)
//
// 目的：设置元件的值
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
// 函数：value()
//
// 目的：获取元件的值
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
// 函数：
//
// 目的：
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
// 函数：append()
//
// 目的：
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
// 函数：each()
//
// 目的：
//
YCQuery& YCQuery::each()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;
	return *this;
}

//
// 函数：hover()
//
// 目的：
//
YCQuery& YCQuery::hover()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：toggle()
//
// 目的：
//
YCQuery& YCQuery::toggle()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：hide()
//
// 目的：
//
YCQuery& YCQuery::hide()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：show()
//
// 目的：
//
YCQuery& YCQuery::show()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：fadeIn()
//
// 目的：
//
YCQuery& YCQuery::fadeIn()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：fadeOut()
//
// 目的：
//
YCQuery& YCQuery::fadeOut()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：
//
// 目的：
//
YCQuery& YCQuery::animation()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：move(const char* speed, int x, int y)
//
// 目的：移动元件中心点到(x, y)
//
YCQuery& YCQuery::move(const char* speed, int x, int y)
{
	RETRIEVE_AND_VALIDATE_SELECTOR;
/*
	int noOfTags = selector->size();
	if (noOfTags != 1)
	{
		LOG_WARNING("YCQuery::move选择器数量不唯一：" << noOfTags);
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
// 函数：
//
// 目的：
//
YCQuery& YCQuery::parent()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：
//
// 目的：
//
YCQuery& YCQuery::child()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：
//
// 目的：
//
YCQuery& YCQuery::click()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：
//
// 目的：
//
YCQuery& YCQuery::submit()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：
//
// 目的：
//
YCQuery& YCQuery::slideUp()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：
//
// 目的：
//
YCQuery& YCQuery::slideDown()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：
//
// 目的：
//
YCQuery& YCQuery::addClass()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：
//
// 目的：
//
YCQuery& YCQuery::removeClass()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	return *this;
}

//
// 函数：end()
//
// 目的：清除selector
//
YCQuery& YCQuery::end()
{
	RETRIEVE_AND_VALIDATE_SELECTOR;

	myContext->updateSelector(NULL);

	return *this;
}
