#include "YCTextUnit.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"

YCTextUnit::YCTextUnit(const char* content, unsigned int length)
	: myColor(0xFFFFFFFF)
	, myUnderline(false)
{
	SMART_ASSERT(content != NULL);
	SMART_ASSERT(length > 0);

	myContent = new char[length+1];
	memcpy(myContent, content, length);
	myContent[length] = '\0';
}


YCTextUnit::~YCTextUnit(void)
{
	SAFE_DELETE(myContent);
}

void YCTextUnit::setColor(unsigned int color)
{
	myColor = color;
}

void YCTextUnit::setUnderline(bool enabled)
{
	myUnderline = enabled;
}
