#include "YCColorBackground.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

YCColorBackground::YCColorBackground(YCIUITag *host,      //宿主UI元件
		                             unsigned int color)  //背景透明度
	: YCIBackground(host)
	, myColor(color)
	, myDrawer(NULL)
{
	myColor += 0xFF000000;
	YCGraphic* graphic = (YCGraphic*)YCRegistry::get("graphic");
	if (graphic == NULL)
	{
		throw YCException(2002, "YCColorBackground无法获取YCGraphic句柄！");
	}
	myDrawer = graphic->createPrimitive();
}

YCColorBackground::~YCColorBackground(void)
{
}

//
// 函数：draw()
//
// 目的：在指定host.(myPositionX, myPositionY)处
//      以透明度alpha绘制host.(myWidth, myHeight)的背景
//
void YCColorBackground::draw(float alpha, int left, int top, int width, int height)
{
	myDrawer->drawRetangle(left, top, left+width, top+height, myColor, myColor, myColor, myColor);
}