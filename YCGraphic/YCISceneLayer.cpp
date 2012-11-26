#include "YCISceneLayer.h"

#include "YCGraphic.h"
#include "YCBasic\YCDList.h"

YCISceneLayer::YCISceneLayer(E_SCENELAYER_ZORDER zOrder)
	: myZOrder(zOrder)
{
}

YCISceneLayer::~YCISceneLayer(void)
{
}


//
// 函数：onLostDevice()
// 
// 目的：当图形系统丢失设备时由YCScene调用
//
void YCISceneLayer::onLostDevice()
{

}

//
// 函数：onResetDevice()
// 
// 目的：当图形系统重置设备时由YCScene调用
//
void YCISceneLayer::onResetDevice()
{

}

//
// 函数：bindGraphic(YCGraphic* graphic)
//
// 目的：绑定图形系统
//       必须在创建成功后，立即调用
//
void YCISceneLayer::bindGraphic(YCGraphic* graphic)
{
	myGraphic = graphic;
}

E_SCENELAYER_ZORDER YCISceneLayer::order()
{
	return myZOrder;
}
