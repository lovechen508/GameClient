#include "YCTiledLayer.h"

#include "YCGraphic\YCMapDef.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

const char* YCTiledLayer::BLOCK_LAYER_NAME = "block";
const char* YCTiledLayer::LEVEL_2_MAP_OBJECT = "mapObject";
const char* YCTiledLayer::LEVEL_2_MAP_SMALL_GROUND = "smallGround";
const char* YCTiledLayer::LEVEL_2_MAP_BIG_GROUND = "bigGround";
const char* YCTiledLayer::LEVEL_1_MAP = "staticGround";

YCTiledLayer::YCTiledLayer(const char* name,
						   unsigned int xTiles,
						   unsigned int yTiles)
	: myName(name)
	, myLayerValue(-1)
	, myXTiles(xTiles)
	, myYTiles(yTiles)
	, myTiledMap(NULL)
	, myGIDs(NULL)
{
	SMART_ASSERT(myXTiles > 0);
	SMART_ASSERT(myYTiles > 0);

	if (myXTiles > 0 && myYTiles > 0)
	{
		myGIDs = new unsigned int[myXTiles * myYTiles];
	}

	if      (strcmp(name, LEVEL_1_MAP) == 0) myLayerValue = LEVEL_1_MAP_LAYER;
	else if (strcmp(name, LEVEL_2_MAP_BIG_GROUND) == 0) myLayerValue = LEVEL_2_MAP_BIG_BG_LAYER;
	else if (strcmp(name, LEVEL_2_MAP_SMALL_GROUND) == 0) myLayerValue = LEVEL_2_MAP_LITTLE_BG_LAYER;
	else if (strcmp(name, BLOCK_LAYER_NAME) == 0) myLayerValue = BLOCK_LAYER;
	else if (strstr(name, LEVEL_2_MAP_OBJECT) != NULL) myLayerValue = OBJECT_LAYER;
}

YCTiledLayer::~YCTiledLayer(void)
{
	SAFE_DELETE(myName);
	SAFE_DELETE_ARRAY(myGIDs);
}

//
// 函数：getLayerValue() const
//
// 目的：取得层的位置值
//
int YCTiledLayer::getLayerValue() const
{
	return myLayerValue;
}

//
// 函数：setProperty(const char* name, const char* value)
//
// 目的：设置Layer属性
//
void YCTiledLayer::setProperty(const char* name, const char* value)
{
	SMART_ASSERT(name != NULL);
	SMART_ASSERT(value != NULL);
	if (name == NULL || value == NULL)
	{
		throw YCException(2002, "YCTiledLayer::setProperty设置无效属性！");
	}

	myProperties[name] = value;
}

//
// 函数：hasProperty(const char* name)
//
// 目的：测试属性是否存在
//
bool YCTiledLayer::hasProperty(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (myProperties.find(name) != myProperties.end());
}

//
// 函数：getProperty(const char* name)
//
// 目的：获取属性
//
const char* YCTiledLayer::getProperty(const char* name)
{
	SMART_ASSERT(name != NULL);
	YCTiledLayer::PROPERTYMAP::const_iterator at = myProperties.find(name);
	if (at != myProperties.end())
	{
		return at->second.c_str();
	}
	return NULL;
}

//
// 函数：bindToMap(YCTiledMap* map)
//
// 目的：绑定到地图
//
void YCTiledLayer::bindToMap(YCTiledMap* map)
{
	myTiledMap = map;
}

//
// 函数：bindGID(unsigned int index, unsigned int gid)
//
// 目的：绑定地图块属性
//
void YCTiledLayer::bindGID(unsigned int index, unsigned int gid)
{
	if (myGIDs == NULL)
	{
		throw YCException(2002, "YCTiledLayer::bindGID地图属性配置错误！");
	}

	SMART_ASSERT(index < (myXTiles*myYTiles));
	if (index >= (myXTiles*myYTiles))
	{
		throw YCException(2002, "YCTiledLayer::bindGID地图属性配置错误, Gid下标越界！");
	}

	myGIDs[index] = gid;
}

//
// 函数：getGID(unsigned int x, unsigned int y)
//
// 目的：取得地图块对应Tileset的GID
//
unsigned int YCTiledLayer::getGID(unsigned int x, unsigned int y)
{
	SMART_ASSERT(x < myXTiles);
	SMART_ASSERT(y < myYTiles);

	if (x >= myXTiles)
	{
		throw YCException(2002, " YCTiledLayer::getGID地图属性获取错误, X下标越界！");
	}

	if (y >= myYTiles)
	{
		throw YCException(2002, " YCTiledLayer::getGID地图属性获取错误, Y下标越界！");
	}

	return ((myGIDs == NULL) ? 0 : myGIDs[y*myXTiles + x]);
}

//
// 函数：isDynamicZOrder()
//
// 目的：是否动态层
//
bool YCTiledLayer::isDynamicZOrder()
{
	return (myZOrder == DYNAMIC_ZORDER);
}

//
// 函数：ZOrder()
//
// 目的：取得ZOrder
//
unsigned int YCTiledLayer::ZOrder()
{
	return myZOrder;
}
