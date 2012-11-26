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
// ������getLayerValue() const
//
// Ŀ�ģ�ȡ�ò��λ��ֵ
//
int YCTiledLayer::getLayerValue() const
{
	return myLayerValue;
}

//
// ������setProperty(const char* name, const char* value)
//
// Ŀ�ģ�����Layer����
//
void YCTiledLayer::setProperty(const char* name, const char* value)
{
	SMART_ASSERT(name != NULL);
	SMART_ASSERT(value != NULL);
	if (name == NULL || value == NULL)
	{
		throw YCException(2002, "YCTiledLayer::setProperty������Ч���ԣ�");
	}

	myProperties[name] = value;
}

//
// ������hasProperty(const char* name)
//
// Ŀ�ģ����������Ƿ����
//
bool YCTiledLayer::hasProperty(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (myProperties.find(name) != myProperties.end());
}

//
// ������getProperty(const char* name)
//
// Ŀ�ģ���ȡ����
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
// ������bindToMap(YCTiledMap* map)
//
// Ŀ�ģ��󶨵���ͼ
//
void YCTiledLayer::bindToMap(YCTiledMap* map)
{
	myTiledMap = map;
}

//
// ������bindGID(unsigned int index, unsigned int gid)
//
// Ŀ�ģ��󶨵�ͼ������
//
void YCTiledLayer::bindGID(unsigned int index, unsigned int gid)
{
	if (myGIDs == NULL)
	{
		throw YCException(2002, "YCTiledLayer::bindGID��ͼ�������ô���");
	}

	SMART_ASSERT(index < (myXTiles*myYTiles));
	if (index >= (myXTiles*myYTiles))
	{
		throw YCException(2002, "YCTiledLayer::bindGID��ͼ�������ô���, Gid�±�Խ�磡");
	}

	myGIDs[index] = gid;
}

//
// ������getGID(unsigned int x, unsigned int y)
//
// Ŀ�ģ�ȡ�õ�ͼ���ӦTileset��GID
//
unsigned int YCTiledLayer::getGID(unsigned int x, unsigned int y)
{
	SMART_ASSERT(x < myXTiles);
	SMART_ASSERT(y < myYTiles);

	if (x >= myXTiles)
	{
		throw YCException(2002, " YCTiledLayer::getGID��ͼ���Ի�ȡ����, X�±�Խ�磡");
	}

	if (y >= myYTiles)
	{
		throw YCException(2002, " YCTiledLayer::getGID��ͼ���Ի�ȡ����, Y�±�Խ�磡");
	}

	return ((myGIDs == NULL) ? 0 : myGIDs[y*myXTiles + x]);
}

//
// ������isDynamicZOrder()
//
// Ŀ�ģ��Ƿ�̬��
//
bool YCTiledLayer::isDynamicZOrder()
{
	return (myZOrder == DYNAMIC_ZORDER);
}

//
// ������ZOrder()
//
// Ŀ�ģ�ȡ��ZOrder
//
unsigned int YCTiledLayer::ZOrder()
{
	return myZOrder;
}
