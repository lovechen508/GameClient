#ifndef _INCLUDE_YCCOLORBACKGROUND_H_
#define _INCLUDE_YCCOLORBACKGROUND_H_

#include "YCIBackground.h"

#include "YCGraphic.h"

/*
 * YCColorBackground : ����ɫΪ������
 *
 * ע��
 */
class YCAPI YCColorBackground : public YCIBackground
{

public:

	YCColorBackground(YCIUITag *host,       //����UIԪ��
		              unsigned int color);  //״̬��ɫ

	virtual ~YCColorBackground(void);

	//
	// ������draw()
	//
	// Ŀ�ģ���ָ��host.(myPositionX, myPositionY)��
	//      ��͸����alpha����host.(myWidth, myHeight)�ı���
	//
	virtual void draw(float alpha, int left, int top, int width, int height);


private:

	unsigned int myColor;

	YCGraphic::YCD3DPrimitive* myDrawer;
};

#endif
