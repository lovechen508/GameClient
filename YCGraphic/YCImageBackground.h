#ifndef _INCLUDE_YCBACKGROUND_H_
#define _INCLUDE_YCBACKGROUND_H_

#include "YCIBackground.h"

#include "YCGraphic.h"

class YCTexture;

/*
 * UI控件背景缺省实现，单张纹理带偏移量
 *
 */

class YCAPI YCImageBackground :
	public YCIBackground
{

public:

	YCImageBackground(YCIUITag *host, YCTexture* texture);

	virtual ~YCImageBackground(void);

	//
	// 函数：draw()
	//
	// 目的：在指定host.(myPositionX, myPositionY)处
	//      以透明度alpha绘制host.(myWidth, myHeight)的背景
	//
	virtual void draw(float alpha, int left, int top, int width, int height);


private:

	YCTexture* myTexture;

	YCGraphic::YCD3DSprite* myDrawer;
};

#endif
