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
// ������draw(unsigned int layer, unsigned int timestamp)
//
// Ŀ�ģ���ĳһ��
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
// ������addObject(YCIMapObject* object)
//
// Ŀ�ģ��ҽӶ��󵽵�ͼ��
//
void YCIMapTile::addObject(YCIMapObject* object)
{
	SMART_ASSERT(object != NULL);
	if (object == NULL)
	{
		throw YCException(2002, "YCIMapTile::addObject�ҽӿն��󵽵�ͼ��");
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
