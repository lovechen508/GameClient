#ifndef _INCLUDE_YCISCENELAYER_H_
#define _INCLUDE_YCISCENELAYER_H_

#include <Windows.h>

#include "YCSceneDef.h"

#include "YCInput\YCIInputListener.h"

class YCGraphic;

/*
 * YCISceneLayer : ���г�����ӿڻ���
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
	// ������bindGraphic(YCGraphic* graphic)
	//
	// Ŀ�ģ���ͼ��ϵͳ
	//       �����ڴ����ɹ�����������
	//
	void bindGraphic(YCGraphic* graphic);

	//
	// ������onLostDevice()
	// 
	// Ŀ�ģ���ͼ��ϵͳ��ʧ�豸ʱ��YCScene����
	//
	void onLostDevice();

	//
	// ������onResetDevice()
	// 
	// Ŀ�ģ���ͼ��ϵͳ�����豸ʱ��YCScene����
	//
	void onResetDevice();

	E_SCENELAYER_ZORDER order();

	//
	// ������render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
	//
	// Ŀ�ģ��������Ⱦ�ӿ�
	//
	virtual void render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY) = 0; 

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

private:

	E_SCENELAYER_ZORDER myZOrder;

	YCGraphic *myGraphic;
};

#endif
