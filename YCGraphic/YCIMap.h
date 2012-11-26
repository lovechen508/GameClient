#ifndef _INCLUDE_YCIMAP_H_
#define _INCLUDE_YCIMAP_H_

class YCIMapTile;
class YCIMapObject;

#include "YCIModel.h"

/*
 * YCIMap : 地图抽象接口类
 *
 * 说明：
 *
 *
 *
 */
class YCAPI YCIMap :
	public YCIModel
{

public:

	YCIMap(unsigned int mapId,
		   unsigned int xTiles,
		   unsigned int yTiles);

	virtual ~YCIMap(void);
		
	//
	// 函数：initialize()
	//
	// 目的：数据模型初始化
	//
	virtual bool initialize() = 0;
	
	//
	// 函数：finialize()
	//
	// 目的：释放数据模型中的资源
	//
	virtual void finialize() = 0;	

	//
	// 函数：id()
	//
	// 目的：地图ID
	//
	unsigned int id() const;

	//
	// 函数：xTiles() const
	//
	// 目的：X方向地图块数
	//
	unsigned int xTiles() const;

	//
	// 函数：yTiles() const
	//
	// 目的：Y方向地图块数
	//
	unsigned int yTiles() const;

	//
	// 函数：setTileAt(YCIMapTile* mapTile, unsigned int x, unsigned int y)
	//
	// 目的：设置坐标位置(x, y)的地图块
	//
	void setTileAt(YCIMapTile* mapTile, unsigned int x, unsigned int y);

	//
	// 函数：getTileAt(unsigned int x, unsigned int y)
	//
	// 目的：取得坐标位置(x, y)的地图块
	//
	YCIMapTile* getTileAt(unsigned int x, unsigned int y);

	//
	// 函数：createTiles()
	//
	// 目的：创建地图
	//
	bool createTiles();

	//
	// 函数：destoryTiles()
	//
	// 目的：释放地图块
	//
	void destoryTiles();
	
	//
	// 函数：setBusy(bool busy)
	//
	// 目的：设置地图是否使用中
	//
	//void setBusy(bool busy);

	//
	// 函数：isBusy()
	//
	// 目的：检查地图是否使用中
	//
	//bool isBusy();

protected:
	
	//
	// 属性：地图ID
	//
	unsigned int myId;

	//
	// 属性：myXTiles
	//
	// 目的：水平方向的地图块数
	//
	unsigned int myXTiles;

	//
	// 属性：myYTiles
	//
	// 目的：垂直方向的地图块数
	//
	unsigned int myYTiles;

private:
	
	//
	// 属性：地图使用中标志
	//
	//bool myBusyFlag;

	//
	// 属性：myMapArray
	//
	// 目的：
	//
	YCIMapTile** myMapArray;
};

#endif

