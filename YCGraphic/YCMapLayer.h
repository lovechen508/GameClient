#ifndef _INCLUDE_YCMAPLAYER_H_
#define _INCLUDE_YCMAPLAYER_H_

#include "YCSceneLayer.h"

class YCIMap;

/*
 *  YCMapLayer :  地图层
 */
class YCMapLayer 
	: public YCSceneLayer
{
public:

	YCMapLayer(YCIMap* map);

	virtual ~YCMapLayer(void);

	//
	// 函数：render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
	//
	// 目的：各层的渲染接口
	//
	virtual void render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY); 

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	//
	// 属性：myMap
	//
	// 目的：层中的地图
	//
	YCIMap* myMap;
};

#endif