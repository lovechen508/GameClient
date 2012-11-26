#include "YCScene.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCISceneLayer.h"
#include "YCGraphic.h"

#include "YCBasic\YCAssert.h"

YCScene::YCScene(YCGraphic *graphic)
	: mySceneLayers(new YCDList())
	, myGraphic(graphic)
	, myXPosition(0)
	, myYPosition(0)
	, myXOffset(0)
	, myYOffset(0)
{
}

static void scene_layer_release(void* item)
{
	YCISceneLayer* layer = (YCISceneLayer*)item;
	SAFE_DELETE(layer);
}

YCScene::~YCScene(void)
{
	if (mySceneLayers)
	{
		mySceneLayers->clear(scene_layer_release);
	}
	SAFE_DELETE(mySceneLayers);
}


//
// ������initLayers()
//
// Ŀ�ģ���ʼ��������
//
bool YCScene::initialize()
{
	return true;
}

//
// ������finalLayers()
//
// Ŀ�ģ��ͷų�����
//
void YCScene::finalize()
{

}

//
// ������setPosition(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
//
// Ŀ�ģ����ó�����������
//
void YCScene::setPosition(unsigned int x,
	                      unsigned int y,
						  unsigned int offsetX,
						  unsigned int offsetY)
{
	myXPosition = x;
	myYPosition = y;
	myXOffset = offsetX;
	myYOffset = offsetY;
}

//
// ������onLostDevice()
// 
// Ŀ�ģ���ͼ��ϵͳ��ʧ�豸ʱ��YCGraphic����
//
void YCScene::onLostDevice()
{
	Item_List_T* it = mySceneLayers->begin(); 
	for (; it != mySceneLayers->end(); it = mySceneLayers->next(it))
	{
		YCISceneLayer* current = (YCISceneLayer*)(mySceneLayers->payload(it));
		current->onLostDevice();
	}
}

//
// ������onResetDevice()
// 
// Ŀ�ģ���ͼ��ϵͳ�����豸ʱ��YCGraphic����
//
void YCScene::onResetDevice()
{
	Item_List_T* it = mySceneLayers->begin(); 
	for (; it != mySceneLayers->end(); it = mySceneLayers->next(it))
	{
		YCISceneLayer* current = (YCISceneLayer*)(mySceneLayers->payload(it));
		current->onResetDevice();
	}
}

void YCScene::addLayer(YCISceneLayer *layer)
{
	Item_List_T* it = mySceneLayers->begin(); 
	for (; it != mySceneLayers->end(); it = mySceneLayers->next(it))
	{
		YCISceneLayer* current = (YCISceneLayer*)(mySceneLayers->payload(it));
		if (current->order() > layer->order())
		{
			break;
		}
		else if (current->order() == layer->order())
		{
			SMART_ASSERT(false).error("����������ͬ��zOrder");
		}
	}
	mySceneLayers->append(layer, it);
}

bool YCScene::removeLayer(YCISceneLayer* layer)
{
	for (Item_List_T* it = mySceneLayers->r_begin(); 
		 it != mySceneLayers->r_end(); 
		 it = mySceneLayers->r_next(it))
	{
		YCISceneLayer* current = (YCISceneLayer*)(mySceneLayers->payload(it));
		if (current == layer)
		{
			mySceneLayers->remove(it);
			SAFE_DELETE(current);
			return true;
		}
	} 
	return false;
}

YCISceneLayer* YCScene::getLayer(E_SCENELAYER_ZORDER zOrder)
{
	for (Item_List_T* it = mySceneLayers->begin(); 
		 it != mySceneLayers->end(); 
		 it = mySceneLayers->next(it))
	{
		YCISceneLayer* layer = (YCISceneLayer*)(mySceneLayers->payload(it));
		if (layer->order() == zOrder)
		{
			return layer;
		}
	} 
	
	return NULL;
}

//
// ������render()
//
// Ŀ�ģ���Ⱦ�����������������㣬��ZOrder��С����˳����Ⱦ
//
void YCScene::render()
{
	for (Item_List_T* it = mySceneLayers->begin(); 
		 it != mySceneLayers->end(); 
		 it = mySceneLayers->next(it))
	{
		YCISceneLayer* layer = (YCISceneLayer*)(mySceneLayers->payload(it));
		layer->render(myXPosition, myYPosition, myXOffset, myYOffset);
	}
}

//
// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCScene::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{	
	for (Item_List_T* it = mySceneLayers->begin(); 
		 it != mySceneLayers->end(); 
		 it = mySceneLayers->next(it))
	{
		YCISceneLayer* layer = (YCISceneLayer*)(mySceneLayers->payload(it));
		if (layer->onMessage(message, wParam, lParam))
		{
			return true;
		}
	}
	return false; 
}