#include "YCSceneLayer.h"

YCSceneLayer::YCSceneLayer(E_SCENELAYER_ZORDER zOrder)
	: YCISceneLayer(zOrder)
{
}

YCSceneLayer::~YCSceneLayer(void)
{
}

//
// ������addSprite(YCISprite* sprite)
//
// Ŀ�ģ����Sprite���ò���
//
void YCSceneLayer::addSprite(YCISprite* sprite)
{

}

//
// ������removeSprite(int id)
//
// Ŀ�ģ��Ӹò����Ƴ�Sprite
//
YCISprite* YCSceneLayer::removeSprite(int id)
{
	return NULL;
}

//
// ������bool handle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCSceneLayer::handle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}
