#include "YCSceneLayer.h"

YCSceneLayer::YCSceneLayer(E_SCENELAYER_ZORDER zOrder)
	: YCISceneLayer(zOrder)
{
}

YCSceneLayer::~YCSceneLayer(void)
{
}

//
// 函数：addSprite(YCISprite* sprite)
//
// 目的：添加Sprite到该层中
//
void YCSceneLayer::addSprite(YCISprite* sprite)
{

}

//
// 函数：removeSprite(int id)
//
// 目的：从该层中移除Sprite
//
YCISprite* YCSceneLayer::removeSprite(int id)
{
	return NULL;
}

//
// 函数：bool handle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCSceneLayer::handle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}
