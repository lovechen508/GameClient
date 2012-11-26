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
// ������setAlphaValue(float alpha)
//
// Ŀ�ģ����ñ���͸����
//
void YCIBackground::setAlphaValue(float alpha)
{
	myAlphaValue = alpha;
}