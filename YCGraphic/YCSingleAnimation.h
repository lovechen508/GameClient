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
	// ������draw(void *pD3Device)
	//
	// Ŀ�ģ����ƶ���
	//
	virtual void draw(void *pD3Device);

};

#endif

