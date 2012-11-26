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
	// ������YCAnimationBackground(YCIUITag* host, YCIAnimation* animation)
	//
	// Ŀ�ģ����캯������Ԫ���Ͷ���
	//	
	YCAnimationBackground(YCIUITag* host, YCIAnimation* animation);
	
	virtual ~YCAnimationBackground(void);

	//
	// ������draw()
	//
	// Ŀ�ģ���ָ��host.(myPositionX, myPositionY)��
	//      ��͸����alpha����host.(myWidth, myHeight)�ı���
	//
	virtual void draw(float alpha, int left, int top, int width, int height);

private:

	YCIAnimation* myAnimation;

	YCGraphic::YCD3DSprite* myDrawer;
};

#endif
