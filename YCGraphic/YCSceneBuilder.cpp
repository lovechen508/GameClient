#include "YCSceneBuilder.h"

#include <memory>

#include "YCScene.h"
#include "YCGraphic.h"
#include "YCUILayer.h"
#include "YCMapLayer.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

/*
 * 创建登录场景
 */
YCScene* YCSceneBuilder::buildLoginScene(YCGraphic* graphic)
{
	return NULL;
}

/*
 * 创建角色选择场景
 */
YCScene* YCSceneBuilder::buildRoleScene(YCGraphic* graphic)
{
	return NULL;
}

/*
 * 创建游戏场景
 */
YCScene* YCSceneBuilder::buildGameScene(YCGraphic* graphic, YCIMap* map)
{
	SMART_ASSERT(map != NULL);

	std::auto_ptr<YCScene> scene(new YCScene(graphic));
	if (!scene->initialize())
	{
		throw YCException(2002, "YCSceneBuilder::buildGameScene初始化场景失败!");
	}

	// 添加地图层
	YCISceneLayer * layer = NULL;
	layer = new YCMapLayer(map);
	scene->addLayer(layer);

	// 添加界面层
	layer = new YCUILayer();
	scene->addLayer(layer);
	return scene.release();
}

/*
 * 创建退出场景
 */ 
YCScene* YCSceneBuilder::buildQuitScene(YCGraphic* graphic)
{
	return NULL;
}