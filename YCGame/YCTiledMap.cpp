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
// 函数：initialize()
//
// 目的：数据模型初始化
//      完成地图的构建
//
bool YCTiledMap::initialize()
{
	if (!createTiles())
	{
		throw YCException(2002, "YCTiledMap::initialize地图初始化失败!");
	}

	YCTextureManager* textureManager = (YCTextureManager*)YCRegistry::get("textureManager");
	if (textureManager == NULL)
	{
		throw YCException(2002, "YCTiledMap::initialize获取TextureManager失败!");
	}

	//准备图块
	for (Item_List_T* item = myTilesets->begin();
		 item != myTilesets->end();
		 item = myTilesets->next(item))
	{
		YCTileSet *tileset = (YCTileSet*)myTilesets->payload(item);
		const char* image = tileset->getImageFile();
		YCTexture* texture = NULL;
		if (image[0] == '.' && image[1] == '.' && image[2] == '/')
		{
			LOG_DEBUG("YCTiledMap::initialize加载图块：" << image+3);
			texture = textureManager->fetch(TEXTURE_MAP, image+3);
		}
		else
		{
			char buf[MAX_PATH] = {0};
			sprintf(buf, "%d/%s", myId, image);
			LOG_DEBUG("YCTiledMap::initialize加载图块：" << buf);
			texture = textureManager->fetch(TEXTURE_MAP, buf);
		}

		if (NULL == texture)
		{
			throw YCException(2002, "YCTiledMap::initialize加载图块失败！", image);
		}

		tileset->setTexture(texture);
	}

	// 遍历所有的层
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

					// 检查GID来自于那个图块
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
// 函数：addTileSet(YCTileSet* tileSet)
//
// 目的：增加图块
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
// 函数：addLayer(YCTiledLayer* layer)
//
// 目的：增加层
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
// 函数：getTileWidth()
//
// 目的：取得地图块宽度
//
unsigned int YCTiledMap::getTileWidth()
{
	return myTileWidth;
}

//
// 函数：getTileHeight()
//
// 目的：取得地图块高度
//
unsigned int YCTiledMap::getTileHeight()
{
	return myTileHeight;
}

//
// 函数：finialize()
//
// 目的：释放数据模型中的资源
//
void YCTiledMap::finialize()
{
	destoryTiles();
}
