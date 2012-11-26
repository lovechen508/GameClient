#include "YCIMapObject.h"

#include "YCBasic\YCException.h"

YCIMapObject::YCIMapObject()
{
}

YCIMapObject::~YCIMapObject()
{
}

//
// 函数：setLayer()
//
// 目的：返回物体所在层
//
void YCIMapObject::setLayer(unsigned int layer)
{
	myLayer = layer;
}

//
// 函数：layer()
//
// 目的：返回物体所在层
//
unsigned int YCIMapObject::getLayer() const
{
	return myLayer;
}

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
int YCIMapObject::compareLayer(const YCIMapObject* other) const
{
	if (other == NULL)
	{
		throw YCException(2002, "YCIMapObject::compareLayer other为NULL");
	}

	if      (other->getLayer() <  myLayer) return 1;
	else if (other->getLayer() >  myLayer) return -1;
	else return extraCompare(other);
}

///////////////////////////////////////////////////
// Private Part
///////////////////////////////////////////////////
	
//
// 函数：extraCompare(const YCIMapObject* other) const
//
// 目的：附加层次比较关系
//
int YCIMapObject::extraCompare(const YCIMapObject* other) const
{
	return 0; // 默认无附加比较
}