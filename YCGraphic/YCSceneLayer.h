#ifndef _INCLUDE_YCSCENELAYER_H_
#define _INCLUDE_YCSCENELAYER_H_

#include "YCISprite.h"
#include "yciscenelayer.h"

class YCDList;

class YCSceneLayer :
	public YCISceneLayer
{
public:
	YCSceneLayer(E_SCENELAYER_ZORDER zOrder);

	virtual ~YCSceneLayer(void);

	//
	// 函数：addSprite(YCISprite* sprite)
	//
	// 目的：添加Sprite到该层中
	//
	void addSprite(YCISprite* sprite);

	//
	// 函数：removeSprite(int id)
	//
	// 目的：从该层中移除Sprite
	//
	YCISprite* removeSprite(int id);
		
	//
	// 函数：bool handle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool handle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:

	YCDList* mySprites;
};

#endif
