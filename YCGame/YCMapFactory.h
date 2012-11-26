#ifndef _INCLUDE_YCMAPFACTORY_H_
#define _INCLUDE_YCMAPFACTORY_H_

#include "YCGraphic\YCIMapFactory.h"

#include <map>

class YCTiledMap;
class YCConfig;

/*
 * YCMapFactory : 
 *
 * 注释 ：地图工厂，缓冲池
 *
 *
 */
class YCMapFactory : public YCIMapFactory
{

public:

	YCMapFactory(void);

	~YCMapFactory(void);

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：初始化地图工厂
	//
	bool initialize(YCConfig& config);

	//
	// 函数：acquire(unsigned int mapId)
	//
	// 目的：获取地图
	//
	virtual YCIMap* acquire(unsigned int mapId);

	//
	// 函数：release(YCMap* map)
	//
	// 目的：释放地图
	//
	//virtual void release(YCIMap* map);

	//
	// 函数：finalize()
	//
	// 目的：释放地图工厂
	//
	void finalize();

private:

	//
	// 属性：myMapDirectory
	//
	// 目的：地图文件存储路径
	//
	std::string myMapDirectory;

	//
	// 属性：myMaps
	//
	// 目的：地图内存镜像
	//
	std::map<unsigned int, YCTiledMap*> myMaps;

};

#endif

