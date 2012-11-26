#ifndef _INCLUDE_YCSINGLEANIMATION_H_
#define _INCLUDE_YCSINGLEANIMATION_H_

#include "YCIAnimation.h"

class YCSingleAnimation :
	public YCIAnimation
{

public:

	YCSingleAnimation(int animationId);

	virtual ~YCSingleAnimation(void);

	//
	// 函数：draw(void *pD3Device)
	//
	// 目的：绘制动画
	//
	virtual void draw(void *pD3Device);

};

#endif

