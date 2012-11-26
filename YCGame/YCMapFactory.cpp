#include "YCMapFactory.h"

#include "YCTiledMap.h"
#include "YCMapBuilder.h"

#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCConfig\YCConfig.h"

YCMapFactory::YCMapFactory(void)
{
}


YCMapFactory::~YCMapFactory(void)
{
	try
	{
		finalize();
	}
	catch (YCException& e)
	{
		LOG_ERROR("地图工厂析构异常：" << e.what());
	}
}

//
// 函数：initialize(YCConfig& config)
//
// 目的：初始化地图工厂
//
bool YCMapFactory::initialize(YCConfig& config)
{
	const char* mapDir = config.tileMapPath();
	if (mapDir == NULL)
	{
		throw YCException(2002, "YCMapFactory::initialize未正确设置地图路径！");
	}
	
	//获取当前工作目录
	char theExePath[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(theExePath, MAX_PATH))
	{
		return false;
	}

	myMapDirectory = theExePath;
	myMapDirectory += "\\..\\";
	myMapDirectory += mapDir;

	return true;
}

//
// 函数：acquire(unsigned int mapId)
//
// 目的：获取地图
//
YCIMap* YCMapFactory::acquire(unsigned int mapId)
{
	YCTiledMap* result = NULL;
/*
	std::map<unsigned int, YCTiledMap*>::iterator at = myMaps.find(mapId);
	if (at == myMaps.end())
	{
		char fullPath[MAX_PATH] = {0};
		sprintf(fullPath, "%s\\%d\\%d.tmx", myMapDirectory.c_str(), mapId, mapId);
		result = YCMapBuilder::build(mapId, fullPath); //TODO ...
		if (result != NULL)
		{
			myMaps[mapId] = result;
			result->initialize();
		}
	}
	else
	{
		result = at->second;
	}

	if (result != NULL)
	{
		result->setBusy(true);
	}
*/
	char fullPath[MAX_PATH] = {0};
	sprintf(fullPath, "%s\\%d\\%d.tmx", myMapDirectory.c_str(), mapId, mapId);
	result = YCMapBuilder::build(mapId, fullPath); //TODO ...
	if (result != NULL)
	{
		myMaps[mapId] = result;
		result->initialize();
	}

	return result;
}
/*
//
// 函数：
//
// 目的：
//
void YCMapFactory::release(YCIMap* map)
{
	SMART_ASSERT(map != NULL);
	SMART_ASSERT(map->isBusy());
	SMART_ASSERT(myMaps.find(map->id()) != myMaps.end());

	map->finialize();
	map->setBusy(false);

}
*/
//
// 函数：
//
// 目的：
//
void YCMapFactory::finalize()
{
	/*
	for (std::map<unsigned int, YCTiledMap*>::iterator it = myMaps.begin();
		 it != myMaps.end();
		 ++it)
	{
		YCIMap* map = it->second;
		if (map->isBusy())
		{
			throw YCException(2002, "地图还在使用中！");
		}

		SAFE_DELETE(map);
	}
	*/
	myMaps.clear();
}
