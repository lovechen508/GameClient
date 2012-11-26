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
// 函数：initLayers()
//
// 目的：初始化场景层
//
bool YCScene::initialize()
{
	return true;
}

//
// 函数：finalLayers()
//
// 目的：释放场景层
//
void YCScene::finalize()
{

}

//
// 函数：setPosition(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
//
// 目的：设置场景的正中心
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
// 函数：onLostDevice()
// 
// 目的：当图形系统丢失设备时由YCGraphic调用
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
// 函数：onResetDevice()
// 
// 目的：当图形系统重置设备时由YCGraphic调用
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
			SMART_ASSERT(false).error("两个层有相同的zOrder");
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
// 函数：render()
//
// 目的：渲染场景，遍历场景各层，按ZOrder由小到大顺序渲染
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
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
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