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
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCUIUl::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (strcmp("li", name) == 0);
}