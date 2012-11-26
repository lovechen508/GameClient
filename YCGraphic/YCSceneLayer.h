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
	// ������addSprite(YCISprite* sprite)
	//
	// Ŀ�ģ����Sprite���ò���
	//
	void addSprite(YCISprite* sprite);

	//
	// ������removeSprite(int id)
	//
	// Ŀ�ģ��Ӹò����Ƴ�Sprite
	//
	YCISprite* removeSprite(int id);
		
	//
	// ������bool handle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool handle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:

	YCDList* mySprites;
};

#endif
