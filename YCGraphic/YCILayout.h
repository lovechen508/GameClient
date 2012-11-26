#ifndef _INCLUDE_YCILAYOUT_H_
#define _INCLUDE_YCILAYOUT_H_

#include "YCBorderLayoutDef.h"

class YCIComponent;

/*
 * YCILayout : UI布局接口类
 *
 *
 */
class YCAPI YCILayout
{
public:

	YCILayout() {}

	virtual ~YCILayout() {}


	//
	// 函数：addComponent(YCIComponent* component, YCBorderLayout position)
	//
	// 目的：添加元件到某特定位置
	//
	virtual void addComponent(YCIComponent* component, YCBorderLayout position) = 0;

	//
	// 函数：removeComponent(YCIComponent* component)
	//
	// 目的：添加元件到某特定位置
	//
	virtual bool removeComponent(YCIComponent* component) = 0;
};


#endif