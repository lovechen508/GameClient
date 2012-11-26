#include "YCTileSet.h"

#include "YCTiledMap.h"

#include "YCGraphic\YCITexture.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCTileSet::YCTileSet(YCTiledMap* map,
	                 unsigned int firstGid,
	                 const char* name,
					 const char* imageFile,
					 unsigned int imageWidth,
					 unsigned int imageHeight)
	: myMap(map)
	, myFirstGId(firstGid)
	, myLastGId(0)
	, myName(name)
	, myTexture(NULL)
	, myImageFile(imageFile)
	, myImageWidth(imageWidth)
	, myImageHeight(imageHeight)
{
	SMART_ASSERT(map != NULL);
	SMART_ASSERT(imageFile != NULL);
	SMART_ASSERT(imageWidth != NULL);
	SMART_ASSERT(imageHeight != NULL);

	myXTiles = myImageWidth / myMap->getTileWidth();
	myYTiles = myImageHeight / myMap->getTileHeight();
	myLastGId = myFirstGId + myXTiles * myYTiles - 1; 
}

YCTileSet::~YCTileSet(void)
{
	SAFE_DELETE(myName);
	SAFE_DELETE(myTexture);
	SAFE_DELETE(myImageFile);
}

//
// 函数：setProperty(unsigned int tid, const char* name, const char* value)
//
// 目的：设置地图块属性
//
void YCTileSet::setProperty(unsigned int tid, const char* name, const char* value)
{
	SMART_ASSERT(name != NULL);
	SMART_ASSERT(value != NULL);
	if (name == NULL || value == NULL)
	{
		throw YCException(2002, "YCTileSet::setProperty设置无效地图块属性");
	}

	myTileProperties[tid].insert(std::pair<std::string, std::string>(name, value));
}

//
// 函数：getName() const
//
// 目的：Tileset块名称
//
const char* YCTileSet::getName() const
{
	return myName;
}

//
// 函数：getFirstGID() const
//
// 目的：第一块Tid
//
unsigned int YCTileSet::getFirstGID() const
{
	return myFirstGId;
}

//
// 函数：getLastGID() const
//
// 目的：末块Tid
//
unsigned int YCTileSet::getLastGID() const
{
	return myLastGId;
}

//
// 函数：hasGID(unsigned int gid) const
//
// 目的：是否有该GID
//
bool YCTileSet::hasGID(unsigned int gid) const
{
	return (gid >= myFirstGId && gid <= myLastGId);
}

//
// 函数：calculatePosition(unsigned int gid, unsigned int& left, unsigned int& top)
//
// 目的：计算该GID在图层中的位置
//
void YCTileSet::calculatePosition(unsigned int gid, unsigned int& left, unsigned int& top)
{
	if (!hasGID(gid))
	{
		throw YCException(2002, "YCTileSet::calculatePosition不在此层中");
	}

	//
	// 因为GID的下标是从1开始，一个3 * 2的地图（3行2列）
	// 获取第3块(第2行第1块)的位置偏移量就是 3 - 1 = 2
	//
	unsigned int offset = gid - myFirstGId;
	unsigned int y = offset / myXTiles;
	unsigned int x = offset % myXTiles;

	left = x * myMap->getTileWidth();
	top = y * myMap->getTileHeight();
}

//
// 函数：getImageFile() const
//
// 目的：图片文件名
//
const char* YCTileSet::getImageFile() const
{
	return myImageFile;
}

//
// 函数：setTexture(YCITexture* texture)
//
// 目的：绑定图片文件的内存纹理
//
void YCTileSet::setTexture(YCITexture* texture)
{
	SMART_ASSERT(myTexture == NULL);
	myTexture = texture;
}

//
// 函数：getTexture(YCITexture* texture)
//
// 目的：获取图片文件的内存纹理
//
YCITexture* YCTileSet::getTexture()
{
	return myTexture;
}

//
// 函数：getImageWidth() const
//
// 目的：图片宽
//
unsigned int YCTileSet::getImageWidth() const
{
	return myImageWidth;
}

//
// 函数：getImageHeight() const
//
// 目的：图片高
//
unsigned int YCTileSet::getImageHeight() const
{
	return myImageHeight;
}

//
// 函数：hasProperty(unsigned int gid) const
//
// 目的：该地图块是否有属性配置
//
bool YCTileSet::hasProperty(unsigned int gid) const
{
	return (myTileProperties.find(gid) != myTileProperties.end());
}

//
// 函数：getProperties(unsigned int gid) const
//
// 目的：取得该块配置的属性
//
const YCTileSet::PROPERTYMAP* YCTileSet::getProperties(unsigned int gid) const
{
	const PROPERTYMAP* result = NULL;

	TILEPROPERTYMAP::const_iterator at = myTileProperties.find(gid);
	if (at != myTileProperties.end())
	{
		result = &(at->second);
	}

	return result;
}
