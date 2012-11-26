#ifndef _INCLUDE_YCBACKGROUND_H_
#define _INCLUDE_YCBACKGROUND_H_

#include "YCIBackground.h"

#include "YCGraphic.h"

class YCTexture;

/*
 * UI�ؼ�����ȱʡʵ�֣����������ƫ����
 *
 */

class YCAPI YCImageBackground :
	public YCIBackground
{

public:

	YCImageBackground(YCIUITag *host, YCTexture* texture);

	virtual ~YCImageBackground(void);

	//
	// ������draw()
	//
	// Ŀ�ģ���ָ��host.(myPositionX, myPositionY)��
	//      ��͸����alpha����host.(myWidth, myHeight)�ı���
	//
	virtual void draw(float alpha, int left, int top, int width, int height);


private:

	YCTexture* myTexture;

	YCGraphic::YCD3DSprite* myDrawer;
};

#endif
