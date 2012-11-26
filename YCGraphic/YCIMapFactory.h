#ifndef _INCLUDE_YCIMAPFACTORY_H_
#define _INCLUDE_YCIMAPFACTORY_H_

class YCIMap;

/*
 * YCIMapFactory : 
 *
 * 注释 ：地图工厂，缓冲池
 *
 *
 */
class YCAPI YCIMapFactory
{

public:

	YCIMapFactory(void) {}

	virtual ~YCIMapFactory(void) {}

	//
	// 函数：acquire(unsigned int mapId)
	//
	// 目的：获取地图
	//
	virtual YCIMap* acquire(unsigned int mapId) = 0;
};

#endif

