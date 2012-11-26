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
	// ȷ�������Ѿ��ͷ����е�ͼ�飡
	//
	destoryTiles();
}

//
// ������id()
//
// Ŀ�ģ���ͼID
//
unsigned int YCIMap::id() const
{
	return myId;
}

//
// ������xTiles() const
//
// Ŀ�ģ�X�����ͼ����
//
unsigned int YCIMap::xTiles() const
{
	return myXTiles;
}

//
// ������yTiles() const
//
// Ŀ�ģ�Y�����ͼ����
//
unsigned int YCIMap::yTiles() const
{
	return myYTiles;
}

/*
//
// ������setBusy(bool busy)
//
// Ŀ�ģ����õ�ͼ�Ƿ�ʹ����
//
void YCIMap::setBusy(bool busy)
{
	myBusyFlag = busy;
}

//
// ������isBusy()
//
// Ŀ�ģ�����ͼ�Ƿ�ʹ����
//
bool YCIMap::isBusy()
{
	return myBusyFlag;
}
*/

//
// ������setTileAt(YCIMapTile* mapTile, unsigned int x, unsigned int y)
//
// Ŀ�ģ���������λ��(x, y)�ĵ�ͼ��
//
void YCIMap::setTileAt(YCIMapTile* mapTile, unsigned int x, unsigned int y)
{
	SMART_ASSERT(mapTile != NULL);
	if (mapTile == NULL)
	{
		throw YCException(2002, "YCIMap::setTileAt���ÿյ�ͼ�飡");
	}

	if (x >= myXTiles)
	{
		throw YCException(2002, "YCIMap::setTileAt���õ�ͼ��X�������");
	}

	if (y >= myYTiles)
	{
		throw YCException(2002, "YCIMap::setTileAt���õ�ͼ��Y�������");
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
// ������getTileAt(unsigned int x, unsigned int y)
//
// Ŀ�ģ�ȡ������λ��(x, y)�ĵ�ͼ��
//
YCIMapTile* YCIMap::getTileAt(unsigned int x, unsigned int y)
{
	unsigned int index = y * myXTiles + x;
	return myMapArray[index];
}

//
// ������createTiles()
//
// Ŀ�ģ�������ͼ
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
// ������destoryTiles()
//
// Ŀ�ģ��ͷŵ�ͼ��
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
