#ifndef _INCLUDE_YCUILAYER_H_
#define _INCLUDE_YCUILAYER_H_

#include "YCISceneLayer.h"

class YCDList;
class YCIComponent;

/*
 * YCUILayer : 界面层
 */
class YCAPI YCUILayer :
	public YCISceneLayer
{
public:

	YCUILayer(void);

	virtual ~YCUILayer(void);

	//
	// 函数：render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
	//
	// 目的：各层的渲染接口
	//
	virtual void render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY); 

	//
	// 函数：addComponent(YCIComponent* component)
	//
	// 目的：添加元件到UI界面层中
	//
	void addComponent(YCIComponent* container);

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	YCDList* myComponents;
};

#endif
