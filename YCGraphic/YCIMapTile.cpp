#include "YCIMapTile.h"

#include "YCIMapObject.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCIMapTile::YCIMapTile(void)
	: myObjects(new YCDList())
{
}

YCIMapTile::~YCIMapTile(void)
{
	if (NULL != myObjects)
	{
		myObjects->clear(&YCDListClearHelper<YCIMapObject>);
	}
	SAFE_DELETE(myObjects);
}

//
// 函数：draw(unsigned int layer, unsigned int timestamp)
//
// 目的：画某一层
//
void YCIMapTile::draw(unsigned int layer, unsigned int timestamp)
{
	for (Item_List_T* item = myObjects->begin();
		 item != myObjects->end();
		 item = myObjects->next(item))
	{
		YCIMapObject* current = (YCIMapObject*)myObjects->payload(item);
		if (current->getLayer() == layer)
		{
			current->draw(timestamp);
		}
	}
}

//
// 函数：addObject(YCIMapObject* object)
//
// 目的：挂接对象到地图块
//
void YCIMapTile::addObject(YCIMapObject* object)
{
	SMART_ASSERT(object != NULL);
	if (object == NULL)
	{
		throw YCException(2002, "YCIMapTile::addObject挂接空对象到地图块");
	}

	Item_List_T* item = myObjects->begin();
	for (; item != myObjects->end(); item = myObjects->next(item))
	{
		YCIMapObject* current = (YCIMapObject*)myObjects->payload(item);
		if (current->compareLayer(object) < 0)
		{
			break;
		}
	}

	myObjects->insert(object, item);
}
