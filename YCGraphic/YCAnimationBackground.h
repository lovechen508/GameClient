#ifndef _INCLUDE_YCANIMATIONBACKGROUND_H_
#define _INCLUDE_YCANIMATIONBACKGROUND_H_

#include "YCIBackground.h"

#include "YCGraphic.h"

class YCIAnimation;

class YCAnimationBackground :
	public YCIBackground
{

public:

	//
	// 函数：YCAnimationBackground(YCIUITag* host, YCIAnimation* animation)
	//
	// 目的：构造函数，绑定元件和动画
	//	
	YCAnimationBackground(YCIUITag* host, YCIAnimation* animation);
	
	virtual ~YCAnimationBackground(void);

	//
	// 函数：draw()
	//
	// 目的：在指定host.(myPositionX, myPositionY)处
	//      以透明度alpha绘制host.(myWidth, myHeight)的背景
	//
	virtual void draw(float alpha, int left, int top, int width, int height);

private:

	YCIAnimation* myAnimation;

	YCGraphic::YCD3DSprite* myDrawer;
};

#endif
