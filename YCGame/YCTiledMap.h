#ifndef _INCLUDE_YCTILEDMAP_H_
#define _INCLUDE_YCTILEDMAP_H_

#include "YCGraphic\YCIMap.h"

class YCDList;
class YCTileSet;
class YCTiledLayer;

/*
 * YCTiledMap : TiledMap 工具生成的地图类
 *
 * 注释：
 *	    <map version="1.0" orientation="orthogonal" width="32" height="60" tilewidth="64" tileheight="32">
 *         <tileset firstgid="1" name="background" tilewidth="64" tileheight="32">
 *             <image source="ice2.png" width="2048" height="1920"/>
 *         </tileset>
 *         <tileset firstgid="1921" name="block" tilewidth="64" tileheight="32">
 *             <image source="../shared/block.png" width="66" height="67"/>
 *             <tile id="0">
 *                 <properties>
 *                     <property name="block" value="1"/>
 *                 </properties>
 *             </tile>
 *          </tileset>
 *          <layer name="background" width="32" height="60">
 *              <data>
 *                   <tile gid="1"/>
 *                   <tile gid="2"/>
 *                   ...
 *              </data>
 *          </layer>
 *          <layer name="block" width="32" height="60">
 *              <data>
 *                    <tile gid="0"/>
 *                    <tile gid="1921" />
 *              </data>
 *          </layer>
 *      </map>
 */
class YCTiledMap :
	public YCIMap
{

public:

	YCTiledMap(unsigned int mapId,
		       unsigned int xTiles,
		       unsigned int yTiles,
			   unsigned int tileWidth,
			   unsigned int tileHeight);

	virtual ~YCTiledMap(void);

	//
	// 函数：initialize()
	//
	// 目的：数据模型初始化
	//
	virtual bool initialize();

	//
	// 函数：addTileSet(YCTileSet* tileSet)
	//
	// 目的：增加图块
	//
	void addTileSet(YCTileSet* tileSet);

	//
	// 函数：addLayer(YCTiledLayer* layer)
	//
	// 目的：增加层
	//
	void addLayer(YCTiledLayer* layer);

	//
	// 函数：getTileWidth()
	//
	// 目的：取得地图块宽度
	//
	unsigned int getTileWidth();

	//
	// 函数：getTileHeight()
	//
	// 目的：取得地图块高度
	//
	unsigned int getTileHeight();

	//
	// 函数：finialize()
	//
	// 目的：释放数据模型中的资源
	//
	virtual void finialize();

private:

	//
	// 属性：myTilesets
	//
	// 目的：图块列表
	//
	YCDList* myTilesets;

	//
	// 属性：myMapLayers
	//
	// 目的：层列表，zorder排序，
	//      越接近链表前端，层越低
	//
	YCDList* myMapLayers;

	//
	// 属性：myTileWidth
	//
	// 目的：地图块宽度
	//
	unsigned int myTileWidth;

	//
	// 属性：myTileHeight
	//
	// 目的：地图块高度
	//
	unsigned int myTileHeight;

};

#endif

