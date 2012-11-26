#include "YCImageBackground.h"

#include "YCTexture.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

YCImageBackground::YCImageBackground(YCIUITag *host, YCTexture* texture)
	: YCIBackground(host)
	, myTexture(texture)
	, myDrawer(NULL)
{
	SMART_ASSERT(texture != NULL);
	YCGraphic* graphic = (YCGraphic*)YCRegistry::get("graphic");
	if (graphic == NULL)
	{
		throw YCException(2002, "YCImageBackground无法获取YCGraphic句柄！");
	}
	myDrawer = graphic->createD3DSprite();
}

YCImageBackground::~YCImageBackground(void)
{
	SAFE_DELETE(myTexture);
}

//
// 函数：draw()
//
// 目的：在指定host.(myPositionX, myPositionY)处
//      以透明度alpha绘制host.(myWidth, myHeight)的背景
//
void YCImageBackground::draw(float alpha, int left, int top, int width, int height)
{
	myDrawer->draw(myTexture, left, top, 1.0f, 1.0f, alpha, 0);
}
