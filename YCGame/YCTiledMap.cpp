#include "YCTiledMap.h"

#include "YCTileSet.h"
#include "YCTiledLayer.h"
#include "YCTiledMapTile.h"
#include "YCTiledTexture.h"

#include "YCGraphic\YCTexture.h"
#include "YCGraphic\YCTextureManager.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

YCTiledMap::YCTiledMap(unsigned int mapId,
	                   unsigned int xTiles,
	                   unsigned int yTiles,
					   unsigned int tileWidth,
					   unsigned int tileHeight)
	: YCIMap(mapId, xTiles, yTiles)
	, myTileWidth(tileWidth)
	, myTileHeight(tileHeight)
	, myTilesets(new YCDList())
	, myMapLayers(new YCDList())
{
	SMART_ASSERT(tileWidth > 0);
	SMART_ASSERT(tileHeight > 0);
}

YCTiledMap::~YCTiledMap(void)
{
	myTilesets->clear(YCDListClearHelper<YCTileSet>);
	SAFE_DELETE(myTilesets);

	myMapLayers->clear(YCDListClearHelper<YCTiledLayer>);
	SAFE_DELETE(myMapLayers);
}

//
// ������initialize()
//
// Ŀ�ģ�����ģ�ͳ�ʼ��
//      ��ɵ�ͼ�Ĺ���
//
bool YCTiledMap::initialize()
{
	if (!createTiles())
	{
		throw YCException(2002, "YCTiledMap::initialize��ͼ��ʼ��ʧ��!");
	}

	YCTextureManager* textureManager = (YCTextureManager*)YCRegistry::get("textureManager");
	if (textureManager == NULL)
	{
		throw YCException(2002, "YCTiledMap::initialize��ȡTextureManagerʧ��!");
	}

	//׼��ͼ��
	for (Item_List_T* item = myTilesets->begin();
		 item != myTilesets->end();
		 item = myTilesets->next(item))
	{
		YCTileSet *tileset = (YCTileSet*)myTilesets->payload(item);
		const char* image = tileset->getImageFile();
		YCTexture* texture = NULL;
		if (image[0] == '.' && image[1] == '.' && image[2] == '/')
		{
			LOG_DEBUG("YCTiledMap::initialize����ͼ�飺" << image+3);
			texture = textureManager->fetch(TEXTURE_MAP, image+3);
		}
		else
		{
			char buf[MAX_PATH] = {0};
			sprintf(buf, "%d/%s", myId, image);
			LOG_DEBUG("YCTiledMap::initialize����ͼ�飺" << buf);
			texture = textureManager->fetch(TEXTURE_MAP, buf);
		}

		if (NULL == texture)
		{
			throw YCException(2002, "YCTiledMap::initialize����ͼ��ʧ�ܣ�", image);
		}

		tileset->setTexture(texture);
	}

	// �������еĲ�
	for (Item_List_T* item = myMapLayers->begin();
		 item != myMapLayers->end();
		 item = myMapLayers->next(item))
	{
		YCTiledLayer* tiledLayer = (YCTiledLayer*)myMapLayers->payload(item);

		for (unsigned int y = 0; y < myYTiles; ++y)
		{
			for (unsigned int x = 0; x < myXTiles; ++x)
			{
				unsigned int gid = tiledLayer->getGID(x, y);
				if (gid > 0)
				{
					YCIMapTile* mapTile = getTileAt(x, y);
					if (mapTile == NULL)
					{
						mapTile = new YCTiledMapTile();
						setTileAt(mapTile, x, y);
					}

					// ���GID�������Ǹ�ͼ��
					for (Item_List_T* tileItem = myTilesets->begin();
						 tileItem != myTilesets->end();
						 tileItem = myTilesets->next(tileItem))
					{
						YCTileSet *tileset = (YCTileSet*)myTilesets->payload(tileItem);
						if (tileset->hasGID(gid))
						{
							YCITexture *texture = tileset->getTexture();
							if (texture != NULL)
							{
								unsigned int left = 0;
								unsigned int top = 0;

								tileset->calculatePosition(gid, left, top);
								YCTiledTexture* tiledTexture = new YCTiledTexture(tileset, texture);
								tiledTexture->setRect(left, top);
								tiledTexture->setLayer(tiledLayer->getLayerValue());
								mapTile->addObject(tiledTexture);
							}

							break;
						}
					}
				}
			}
		}
	}

	return true;
}

//
// ������addTileSet(YCTileSet* tileSet)
//
// Ŀ�ģ�����ͼ��
//
void YCTiledMap::addTileSet(YCTileSet* tileSet)
{
	SMART_ASSERT(tileSet != NULL);
	if (tileSet != NULL)
	{
		myTilesets->append(tileSet, NULL);
	}
}

//
// ������addLayer(YCTiledLayer* layer)
//
// Ŀ�ģ����Ӳ�
//
void YCTiledMap::addLayer(YCTiledLayer* layer)
{
	SMART_ASSERT(layer != NULL);
	if (layer != NULL)
	{
		myMapLayers->append(layer, NULL);
	}
}

//
// ������getTileWidth()
//
// Ŀ�ģ�ȡ�õ�ͼ����
//
unsigned int YCTiledMap::getTileWidth()
{
	return myTileWidth;
}

//
// ������getTileHeight()
//
// Ŀ�ģ�ȡ�õ�ͼ��߶�
//
unsigned int YCTiledMap::getTileHeight()
{
	return myTileHeight;
}

//
// ������finialize()
//
// Ŀ�ģ��ͷ�����ģ���е���Դ
//
void YCTiledMap::finialize()
{
	destoryTiles();
}
