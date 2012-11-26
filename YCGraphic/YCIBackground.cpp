#include "YCIBackground.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"

YCIBackground::YCIBackground(YCIUITag *host)
	: myHost(host)
	, myAlphaValue(1.0f)
{
	SMART_ASSERT(host != NULL);
}

YCIBackground::~YCIBackground(void)
{
}

//
// 函数：setAlphaValue(float alpha)
//
// 目的：设置背景透明度
//
void YCIBackground::setAlphaValue(float alpha)
{
	myAlphaValue = alpha;
}