#include "YCMapLayer.h"

#include "YCIMap.h"

YCMapLayer::YCMapLayer(YCIMap* map)
	: YCSceneLayer(BIG_BACKGROUND_2)
	, myMap(map)
{

}

YCMapLayer::~YCMapLayer(void)
{
	SAFE_DELETE(myMap);
}

//
// ������render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
//
// Ŀ�ģ��������Ⱦ�ӿ�
//
void YCMapLayer::render(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
{
	
}

//
// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCMapLayer::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}
