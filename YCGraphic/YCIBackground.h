#ifndef _INCLUDE_YCIBACKGROUND_H_
#define _INCLUDE_YCIBACKGROUND_H_

#include "YCIUITag.h"

/*
 * 窗口背景接口类
 */

class YCAPI YCIBackground
{
public:
	YCIBackground(YCIUITag *host);

	virtual ~YCIBackground(void);

	//
	// 函数：setAlphaValue(float alpha)
	//
	// 目的：设置背景透明度
	//
	void setAlphaValue(float alpha);

	//
	// 函数：draw()
	//
	// 目的：在指定host.(myPositionX, myPositionY)处
	//      以透明度alpha绘制host.(myWidth, myHeight)的背景
	//
	virtual void draw(float alpha, int left, int top,
		              int width, int height) = 0;

private:

	YCIUITag *myHost;

	float myAlphaValue;
};

#endif
