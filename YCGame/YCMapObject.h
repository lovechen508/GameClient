#ifndef _INCLUDE_YCMAPOBJECT_H_
#define _INCLUDE_YCMAPOBJECT_H_

#include "YCGraphic\YCIMapObject.h"

/*
 * YCMapObject: 地图对象
 *
 * 说明：凡是在地图上出现的对象
 *
 *
 */
class YCMapObject
	: public YCIMapObject
{
public:

	YCMapObject(void);

	virtual ~YCMapObject(void);

	//
	// 函数：draw()
	//
	// 目的：在地图上完成自身的绘制
	//
	virtual void draw();

};

#endif

