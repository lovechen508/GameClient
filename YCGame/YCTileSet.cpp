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
// ������setProperty(unsigned int tid, const char* name, const char* value)
//
// Ŀ�ģ����õ�ͼ������
//
void YCTileSet::setProperty(unsigned int tid, const char* name, const char* value)
{
	SMART_ASSERT(name != NULL);
	SMART_ASSERT(value != NULL);
	if (name == NULL || value == NULL)
	{
		throw YCException(2002, "YCTileSet::setProperty������Ч��ͼ������");
	}

	myTileProperties[tid].insert(std::pair<std::string, std::string>(name, value));
}

//
// ������getName() const
//
// Ŀ�ģ�Tileset������
//
const char* YCTileSet::getName() const
{
	return myName;
}

//
// ������getFirstGID() const
//
// Ŀ�ģ���һ��Tid
//
unsigned int YCTileSet::getFirstGID() const
{
	return myFirstGId;
}

//
// ������getLastGID() const
//
// Ŀ�ģ�ĩ��Tid
//
unsigned int YCTileSet::getLastGID() const
{
	return myLastGId;
}

//
// ������hasGID(unsigned int gid) const
//
// Ŀ�ģ��Ƿ��и�GID
//
bool YCTileSet::hasGID(unsigned int gid) const
{
	return (gid >= myFirstGId && gid <= myLastGId);
}

//
// ������calculatePosition(unsigned int gid, unsigned int& left, unsigned int& top)
//
// Ŀ�ģ������GID��ͼ���е�λ��
//
void YCTileSet::calculatePosition(unsigned int gid, unsigned int& left, unsigned int& top)
{
	if (!hasGID(gid))
	{
		throw YCException(2002, "YCTileSet::calculatePosition���ڴ˲���");
	}

	//
	// ��ΪGID���±��Ǵ�1��ʼ��һ��3 * 2�ĵ�ͼ��3��2�У�
	// ��ȡ��3��(��2�е�1��)��λ��ƫ�������� 3 - 1 = 2
	//
	unsigned int offset = gid - myFirstGId;
	unsigned int y = offset / myXTiles;
	unsigned int x = offset % myXTiles;

	left = x * myMap->getTileWidth();
	top = y * myMap->getTileHeight();
}

//
// ������getImageFile() const
//
// Ŀ�ģ�ͼƬ�ļ���
//
const char* YCTileSet::getImageFile() const
{
	return myImageFile;
}

//
// ������setTexture(YCITexture* texture)
//
// Ŀ�ģ���ͼƬ�ļ����ڴ�����
//
void YCTileSet::setTexture(YCITexture* texture)
{
	SMART_ASSERT(myTexture == NULL);
	myTexture = texture;
}

//
// ������getTexture(YCITexture* texture)
//
// Ŀ�ģ���ȡͼƬ�ļ����ڴ�����
//
YCITexture* YCTileSet::getTexture()
{
	return myTexture;
}

//
// ������getImageWidth() const
//
// Ŀ�ģ�ͼƬ��
//
unsigned int YCTileSet::getImageWidth() const
{
	return myImageWidth;
}

//
// ������getImageHeight() const
//
// Ŀ�ģ�ͼƬ��
//
unsigned int YCTileSet::getImageHeight() const
{
	return myImageHeight;
}

//
// ������hasProperty(unsigned int gid) const
//
// Ŀ�ģ��õ�ͼ���Ƿ�����������
//
bool YCTileSet::hasProperty(unsigned int gid) const
{
	return (myTileProperties.find(gid) != myTileProperties.end());
}

//
// ������getProperties(unsigned int gid) const
//
// Ŀ�ģ�ȡ�øÿ����õ�����
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
