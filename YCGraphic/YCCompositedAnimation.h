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
	// ������draw(void *pD3Device)
	//
	// Ŀ�ģ����ƶ���
	//
	virtual void draw(void *pD3Device);

	//
	// ������bindAnimation(YCIAnimation* animation)
	//
	// Ŀ�ģ����Ӷ�������������
	//
	void bindAnimation(YCIAnimation* animation);

	//
	// ������unbindAnimation(int animationId)
	//
	// Ŀ�ģ��Ӷ��������Ƴ�����
	//
	void unbindAnimation(int animationId);

private:

	YCDList *myAnimationList;

};

#endif

