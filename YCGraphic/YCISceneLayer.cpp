#include "YCISceneLayer.h"

#include "YCGraphic.h"
#include "YCBasic\YCDList.h"

YCISceneLayer::YCISceneLayer(E_SCENELAYER_ZORDER zOrder)
	: myZOrder(zOrder)
{
}

YCISceneLayer::~YCISceneLayer(void)
{
}


//
// ������onLostDevice()
// 
// Ŀ�ģ���ͼ��ϵͳ��ʧ�豸ʱ��YCScene����
//
void YCISceneLayer::onLostDevice()
{

}

//
// ������onResetDevice()
// 
// Ŀ�ģ���ͼ��ϵͳ�����豸ʱ��YCScene����
//
void YCISceneLayer::onResetDevice()
{

}

//
// ������bindGraphic(YCGraphic* graphic)
//
// Ŀ�ģ���ͼ��ϵͳ
//       �����ڴ����ɹ�����������
//
void YCISceneLayer::bindGraphic(YCGraphic* graphic)
{
	myGraphic = graphic;
}

E_SCENELAYER_ZORDER YCISceneLayer::order()
{
	return myZOrder;
}
