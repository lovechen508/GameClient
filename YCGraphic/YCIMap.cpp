#include "YCIMap.h"

#include "YCIMapTile.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCIMap::YCIMap(unsigned int mapId,
		       unsigned int xTiles,
		       unsigned int yTiles)
	: myId(mapId)
	, myXTiles(xTiles)
	, myYTiles(yTiles)
	, myMapArray(NULL)
//	, myBusyFlag(false)
{
}


YCIMap::~YCIMap(void)
{
	//
	// 确保子类已经释放所有地图块！
	//
	destoryTiles();
}

//
// 函数：id()
//
// 目的：地图ID
//
unsigned int YCIMap::id() const
{
	return myId;
}

//
// 函数：xTiles() const
//
// 目的：X方向地图块数
//
unsigned int YCIMap::xTiles() const
{
	return myXTiles;
}

//
// 函数：yTiles() const
//
// 目的：Y方向地图块数
//
unsigned int YCIMap::yTiles() const
{
	return myYTiles;
}

/*
//
// 函数：setBusy(bool busy)
//
// 目的：设置地图是否使用中
//
void YCIMap::setBusy(bool busy)
{
	myBusyFlag = busy;
}

//
// 函数：isBusy()
//
// 目的：检查地图是否使用中
//
bool YCIMap::isBusy()
{
	return myBusyFlag;
}
*/

//
// 函数：setTileAt(YCIMapTile* mapTile, unsigned int x, unsigned int y)
//
// 目的：设置坐标位置(x, y)的地图块
//
void YCIMap::setTileAt(YCIMapTile* mapTile, unsigned int x, unsigned int y)
{
	SMART_ASSERT(mapTile != NULL);
	if (mapTile == NULL)
	{
		throw YCException(2002, "YCIMap::setTileAt设置空地图块！");
	}

	if (x >= myXTiles)
	{
		throw YCException(2002, "YCIMap::setTileAt设置地图块X坐标错误！");
	}

	if (y >= myYTiles)
	{
		throw YCException(2002, "YCIMap::setTileAt设置地图块Y坐标错误！");
	}

	unsigned int index = y * myXTiles + x;
	YCIMapTile *old = myMapArray[index];
	if (old != NULL)
	{
		SAFE_DELETE(old);
	}

	myMapArray[index] = mapTile;
}

//
// 函数：getTileAt(unsigned int x, unsigned int y)
//
// 目的：取得坐标位置(x, y)的地图块
//
YCIMapTile* YCIMap::getTileAt(unsigned int x, unsigned int y)
{
	unsigned int index = y * myXTiles + x;
	return myMapArray[index];
}

//
// 函数：createTiles()
//
// 目的：创建地图
//
bool YCIMap::createTiles()
{
	SMART_ASSERT(myMapArray == NULL);

	unsigned int total = myXTiles * myYTiles;
	myMapArray = new YCIMapTile*[total];
	memset(myMapArray, 0, sizeof(YCIMapTile*) * total);

	return true;
}

//
// 函数：destoryTiles()
//
// 目的：释放地图块
//
void YCIMap::destoryTiles()
{
	if (NULL != myMapArray)
	{
		for (unsigned int y = 0; y < myYTiles; y++)
		{
			for (unsigned int x = 0; x < myXTiles; x++)
			{
				unsigned int index = y*myYTiles + x;
				SAFE_DELETE(myMapArray[index]);
			}
		}

		SAFE_DELETE_ARRAY(myMapArray);
	}	
}
