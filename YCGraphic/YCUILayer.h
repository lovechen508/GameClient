#ifndef _INCLUDE_YCUILAYER_H_
#define _INCLUDE_YCUILAYER_H_

#include "YCISceneLayer.h"

class YCDList;
class YCIComponent;

/*
 * YCUILayer : �����
 */
class YCAPI YCUILayer :
	public YCISceneLayer
{
public:

	YCUILayer(void);

	virtual ~YCUILayer(void);

	//
	// ������render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
	//
	// Ŀ�ģ��������Ⱦ�ӿ�
	//
	virtual void render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY); 

	//
	// ������addComponent(YCIComponent* component)
	//
	// Ŀ�ģ����Ԫ����UI�������
	//
	void addComponent(YCIComponent* container);

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	YCDList* myComponents;
};

#endif
