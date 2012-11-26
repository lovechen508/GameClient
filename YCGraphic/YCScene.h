#ifndef _INCLUDE_YCSCENE_H_
#define _INCLUDE_YCSCENE_H_

#include <Windows.h>

#include "YCSceneDef.h"

#include "YCInput\YCIInputListener.h"

class YCDList;
class YCAudio;
struct YCSpriteChain;
class YCISceneLayer;
class YCGraphic;

class YCAPI YCScene
	: public YCIInputListener
{
public:
	YCScene(YCGraphic *graphic);

	virtual ~YCScene(void);

	//
	// 函数：setPosition(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
	//
	// 目的：设置场景的正中心
	//
	void setPosition(unsigned int x,
		             unsigned int y,
					 unsigned int offsetX,
					 unsigned int offsetY);
				
	//
	// 函数：initialize()
	//
	// 目的：初始化场景层
	//
	bool initialize();

	//
	// 函数：finalize()
	//
	// 目的：释放场景层
	//
	void finalize();

	//
	// 函数：onLostDevice()
	// 
	// 目的：当图形系统丢失设备时由YCGraphic调用
	//
	void onLostDevice();

	//
	// 函数：onResetDevice()
	// 
	// 目的：当图形系统重置设备时由YCGraphic调用
	//
	void onResetDevice();

	//
	// 函数：addLayer(YCISceneLayer *layer)
	//
	// 目的：
	//
	void addLayer(YCISceneLayer *layer);

	//
	// 函数：removeLayer(YCISceneLayer *layer)
	//
	// 目的：从场景中移除一层，注意layer内存已经释放
	//      函数返回true后不应再使用layer
	//
	bool removeLayer(YCISceneLayer* layer);

	//
	// 函数：getLayer(E_SCENELAYER_ZORDER zOrder)
	//
	// 目的：
	//
	YCISceneLayer* getLayer(E_SCENELAYER_ZORDER zOrder);

	//
	// 函数：render()
	//
	// 目的：渲染场景，遍历场景各层，按ZOrder由小到大顺序渲染
	//
	virtual void render();

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	//
	// 属性：myXPosition
	//       myYPosition
	//       myXOffset
	//       myYOffset
	//
	// 目的：场景中心点地图块坐标(x, y)
	//       并且在该块内的偏移量
	//
	unsigned int myXPosition;
	unsigned int myYPosition;
	unsigned int myXOffset;
	unsigned int myYOffset;

	/*
	 * Zordered 场景层
	 *	距离用户越远越靠后
	 *	渲染的时候从后往前渲染
	 *	被遮挡的人物可以在后面50% Alpha再渲染一次
	 *	常见链表顺序为：
	 *    UI -> 人物50% -> 遮挡 -> 人物 -> 小地表 -> 大地表 -> 大地表(双层地表）
	 */
	YCDList* mySceneLayers;
	
	YCAudio * myAudio;
	
	YCSpriteChain *mySpriteHead;

	YCGraphic * myGraphic;
};

#endif
