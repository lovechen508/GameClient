#ifndef _INCLUDE_YCISCENELAYER_H_
#define _INCLUDE_YCISCENELAYER_H_

#include <Windows.h>

#include "YCSceneDef.h"

#include "YCInput\YCIInputListener.h"

class YCGraphic;

/*
 * YCISceneLayer : 所有场景层接口基类
 *
 *					                      -----------------------------
 *				                         |        YCISceneLayer        |
 *				                         |-----------------------------|
 *                                       | void render() = 0           |
 *                                       | E_SCENELAYER_ZORDER order() |
 *                                       | bool handle(HWND hWnd, ...) |
 *                                        -----------------------------
 *                                                      |
 *                                  -------------------------------------------
 *                                 |                                           |
 *                      -----------------------                 -----------------------------
 *                     |    YCSceneLayer       |               |           YCUILayer         |
 *                     |-----------------------|               |-----------------------------|
 *                     | addSprite(YCISprite*) |               | addComponent(YCIComponent*) |
 *                      -----------------------                 -----------------------------
 *                                 |
 *                           ------------    
 *                          | YCMapLayer | 
 *                           ------------   
 *
 */

class YCAPI YCISceneLayer
	: public YCIInputListener
{
public:

	YCISceneLayer(E_SCENELAYER_ZORDER zOrder);

	virtual ~YCISceneLayer(void);

	//
	// 函数：bindGraphic(YCGraphic* graphic)
	//
	// 目的：绑定图形系统
	//       必须在创建成功后，立即调用
	//
	void bindGraphic(YCGraphic* graphic);

	//
	// 函数：onLostDevice()
	// 
	// 目的：当图形系统丢失设备时由YCScene调用
	//
	void onLostDevice();

	//
	// 函数：onResetDevice()
	// 
	// 目的：当图形系统重置设备时由YCScene调用
	//
	void onResetDevice();

	E_SCENELAYER_ZORDER order();

	//
	// 函数：render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
	//
	// 目的：各层的渲染接口
	//
	virtual void render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY) = 0; 

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

private:

	E_SCENELAYER_ZORDER myZOrder;

	YCGraphic *myGraphic;
};

#endif
