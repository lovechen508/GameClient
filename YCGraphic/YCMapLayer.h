#ifndef _INCLUDE_YCMAPLAYER_H_
#define _INCLUDE_YCMAPLAYER_H_

#include "YCSceneLayer.h"

class YCIMap;

/*
 *  YCMapLayer :  ��ͼ��
 */
class YCMapLayer 
	: public YCSceneLayer
{
public:

	YCMapLayer(YCIMap* map);

	virtual ~YCMapLayer(void);

	//
	// ������render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
	//
	// Ŀ�ģ��������Ⱦ�ӿ�
	//
	virtual void render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY); 

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	//
	// ���ԣ�myMap
	//
	// Ŀ�ģ����еĵ�ͼ
	//
	YCIMap* myMap;
};

#endif