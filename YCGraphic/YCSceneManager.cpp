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
// ������registerMapFactory(YCIMapFactory* mapFactory)
//
// Ŀ�ģ�ע���ͼ����
//
void YCSceneManager::registerMapFactory(YCIMapFactory* mapFactory)
{
	SMART_ASSERT(mapFactory != NULL);
	myMapFactory = mapFactory;
}

//
// ������requireScene(YCIPackageResponse* response)
//
// Ŀ�ģ����󳡾�
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
// ������releaseScene(YCScene* scene)
//
// Ŀ�ģ��ͷų���
//
void YCSceneManager::releaseScene(YCScene* scene)
{
	if (scene != NULL)
	{
		scene->finalize();
		SAFE_DELETE(scene);
	}
}