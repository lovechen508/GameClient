#include "YCUIUl.h"

#include "YCBasic\YCAssert.h"

YCUIUl::YCUIUl(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}

YCUIUl::~YCUIUl(void)
{

}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUIUl::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (strcmp("li", name) == 0);
}