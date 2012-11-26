#ifndef _INCLUDE_YCIMAPOBJECT_H_
#define _INCLUDE_YCIMAPOBJECT_H_

#include "YCISprite.h"

/*
 * YCIMapObject : 可放置于地图上对象接口类
 *
 * 说明：
 *
 *
 *
 */
class YCAPI YCIMapObject : public YCISprite
{
public:

	YCIMapObject();

	virtual ~YCIMapObject();

	//
	// 函数：setLayer()
	//
	// 目的：返回物体所在层
	//
	void setLayer(unsigned int layer);

	//
	// 函数：getLayer()
	//
	// 目的：返回物体所在层
	//
	unsigned int getLayer() const;

	//
	// 函数：compareLayer(const YCIMapObject* other) const
	//
	// 目的：比较物体间的相互层次关系
	//      
	// 返回值：
	//        1 : 自身在前
	//        0 : 同层
	//       -1 ： other在前
	//
	int compareLayer(const YCIMapObject* other) const;

private:

	//
	// 函数：extraCompare(const YCIMapObject* other) const
	//
	// 目的：附加层次比较关系
	//
	virtual int extraCompare(const YCIMapObject* other) const;

protected:

	//
	// 属性：myLayer
	//
	// 目的：对象所在层
	//
	unsigned int myLayer;
};

#endif