#ifndef _INCLUDE_YCILAYOUT_H_
#define _INCLUDE_YCILAYOUT_H_

#include "YCBorderLayoutDef.h"

class YCIComponent;

/*
 * YCILayout : UI���ֽӿ���
 *
 *
 */
class YCAPI YCILayout
{
public:

	YCILayout() {}

	virtual ~YCILayout() {}


	//
	// ������addComponent(YCIComponent* component, YCBorderLayout position)
	//
	// Ŀ�ģ����Ԫ����ĳ�ض�λ��
	//
	virtual void addComponent(YCIComponent* component, YCBorderLayout position) = 0;

	//
	// ������removeComponent(YCIComponent* component)
	//
	// Ŀ�ģ����Ԫ����ĳ�ض�λ��
	//
	virtual bool removeComponent(YCIComponent* component) = 0;
};


#endif