#ifndef _INCLUDE_YCCOLORBACKGROUND_H_
#define _INCLUDE_YCCOLORBACKGROUND_H_

#include "YCIBackground.h"

#include "YCGraphic.h"

/*
 * YCColorBackground : 以颜色为背景类
 *
 * 注释
 */
class YCAPI YCColorBackground : public YCIBackground
{

public:

	YCColorBackground(YCIUITag *host,       //宿主UI元件
		              unsigned int color);  //状态颜色

	virtual ~YCColorBackground(void);

	//
	// 函数：draw()
	//
	// 目的：在指定host.(myPositionX, myPositionY)处
	//      以透明度alpha绘制host.(myWidth, myHeight)的背景
	//
	virtual void draw(float alpha, int left, int top, int width, int height);


private:

	unsigned int myColor;

	YCGraphic::YCD3DPrimitive* myDrawer;
};

#endif
