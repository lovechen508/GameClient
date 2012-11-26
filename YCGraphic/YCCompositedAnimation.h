#ifndef _INCLUDE_YCCOMPOSITEDANIMATION_H_
#define _INCLUDE_YCCOMPOSITEDANIMATION_H_

#include "ycianimation.h"

class YCDList;

class YCCompositedAnimation :
	public YCIAnimation
{

public:

	YCCompositedAnimation(int animationId);

	virtual ~YCCompositedAnimation(void);

	//
	// 函数：draw(void *pD3Device)
	//
	// 目的：绘制动画
	//
	virtual void draw(void *pD3Device);

	//
	// 函数：bindAnimation(YCIAnimation* animation)
	//
	// 目的：增加动画到动画组中
	//
	void bindAnimation(YCIAnimation* animation);

	//
	// 函数：unbindAnimation(int animationId)
	//
	// 目的：从动画组中移除动画
	//
	void unbindAnimation(int animationId);

private:

	YCDList *myAnimationList;

};

#endif

