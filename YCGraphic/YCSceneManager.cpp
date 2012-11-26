#include "YCSceneManager.h"

#include "YCScene.h"
#include "YCIMapFactory.h"
#include "YCSceneBuilder.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCNetwork\YCMSGDefine.h"
#include "YCNetwork\YCIPackageResponse.h"

YCSceneManager::YCSceneManager(YCGraphic* graphic)
	: myGraphic(graphic)
	, myMapFactory(NULL)
{
	SMART_ASSERT(graphic != NULL);
}

YCSceneManager::~YCSceneManager()
{
}

//
// 函数：registerMapFactory(YCIMapFactory* mapFactory)
//
// 目的：注册地图工厂
//
void YCSceneManager::registerMapFactory(YCIMapFactory* mapFactory)
{
	SMART_ASSERT(mapFactory != NULL);
	myMapFactory = mapFactory;
}

//
// 函数：requireScene(YCIPackageResponse* response)
//
// 目的：请求场景
//
YCScene* YCSceneManager::requireScene(YCIPackageResponse* response)
{
	SMART_ASSERT(response != NULL);
	//if (response->msgId == );

	// TODO ...
	YCIMap* map = NULL;
	if (myMapFactory != NULL)
	{
		map = myMapFactory->acquire(1);
	}
	return YCSceneBuilder::buildGameScene(myGraphic, map);
}

//
// 函数：releaseScene(YCScene* scene)
//
// 目的：释放场景
//
void YCSceneManager::releaseScene(YCScene* scene)
{
	if (scene != NULL)
	{
		scene->finalize();
		SAFE_DELETE(scene);
	}
}