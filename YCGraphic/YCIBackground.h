#ifndef _INCLUDE_YCIBACKGROUND_H_
#define _INCLUDE_YCIBACKGROUND_H_

#include "YCIUITag.h"

/*
 * ���ڱ����ӿ���
 */

class YCAPI YCIBackground
{
public:
	YCIBackground(YCIUITag *host);

	virtual ~YCIBackground(void);

	//
	// ������setAlphaValue(float alpha)
	//
	// Ŀ�ģ����ñ���͸����
	//
	void setAlphaValue(float alpha);

	//
	// ������draw()
	//
	// Ŀ�ģ���ָ��host.(myPositionX, myPositionY)��
	//      ��͸����alpha����host.(myWidth, myHeight)�ı���
	//
	virtual void draw(float alpha, int left, int top,
		              int width, int height) = 0;

private:

	YCIUITag *myHost;

	float myAlphaValue;
};

#endif
