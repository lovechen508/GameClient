#include "YCMapLayer.h"

#include "YCIMap.h"

YCMapLayer::YCMapLayer(YCIMap* map)
	: YCSceneLayer(BIG_BACKGROUND_2)
	, myMap(map)
{

}

YCMapLayer::~YCMapLayer(void)
{
	SAFE_DELETE(myMap);
}

//
// 函数：render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
//
// 目的：各层的渲染接口
//
void YCMapLayer::render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
{
	
}

//
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCMapLayer::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}
