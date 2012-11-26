#include "YCAnimationBackground.h"

#include "YCIAnimation.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

//
// 函数：YCAnimationBackground(YCIUITag* host, YCIAnimation* animation)
//
// 目的：构造函数，绑定元件和动画
//
YCAnimationBackground::YCAnimationBackground(YCIUITag* host, YCIAnimation* animation)
	: YCIBackground(host)
	, myAnimation(animation)
	, myDrawer(NULL)
{
	SMART_ASSERT(myAnimation != NULL);
	YCGraphic* graphic = (YCGraphic*)YCRegistry::get("graphic");
	if (graphic == NULL)
	{
		throw YCException(2002, "YCAnimationBackground无法获取YCGraphic句柄！");
	}
	myDrawer = graphic->createD3DSprite();
}


YCAnimationBackground::~YCAnimationBackground(void)
{
	SAFE_DELETE(myAnimation);
}

//
// 函数：draw()
//
// 目的：在指定host.(myPositionX, myPositionY)处
//      以透明度alpha绘制host.(myWidth, myHeight)的背景
//
void YCAnimationBackground::draw(float alpha, int left, int top, int width, int height)
{

}