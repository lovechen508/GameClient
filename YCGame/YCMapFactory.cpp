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
		LOG_ERROR("��ͼ���������쳣��" << e.what());
	}
}

//
// ������initialize(YCConfig& config)
//
// Ŀ�ģ���ʼ����ͼ����
//
bool YCMapFactory::initialize(YCConfig& config)
{
	const char* mapDir = config.tileMapPath();
	if (mapDir == NULL)
	{
		throw YCException(2002, "YCMapFactory::initializeδ��ȷ���õ�ͼ·����");
	}
	
	//��ȡ��ǰ����Ŀ¼
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
// ������acquire(unsigned int mapId)
//
// Ŀ�ģ���ȡ��ͼ
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
// ������
//
// Ŀ�ģ�
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
// ������
//
// Ŀ�ģ�
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
			throw YCException(2002, "��ͼ����ʹ���У�");
		}

		SAFE_DELETE(map);
	}
	*/
	myMaps.clear();
}
