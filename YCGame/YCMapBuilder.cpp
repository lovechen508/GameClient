#include "YCMapBuilder.h"

#include "YCTiledMap.h"
#include "YCTileSet.h"
#include "YCTiledLayer.h"

#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCConfig\YCXmlReader.h"

/////////////////////////////////////
// YCTmxHelper Tmx解析内部类
/////////////////////////////////////
class YCTmxHelper
{
public:

	YCTmxHelper() : myMap(NULL)
	{
	}

	YCTiledMap* parse(unsigned int mapId, const char* mapFile)
	{
		if (NULL == mapFile)
		{
			throw YCException(2002, "YCTiledMap::parse无效文件名");
		}

		YCXmlReader reader(mapFile);

		char mapPath[MAX_PATH] = {0};
		if (!YCFileUtil::GetFilePath(mapFile, mapPath, MAX_PATH))
		{
			throw YCException(2002, "YCTmxHelper::parse无法取得地图文件所在路径！");
		}

		myPath = mapPath;

		int xTiles = 0;
		int yTiles = 0;
		int tileWidth = 0;
		int tileHeight = 0;
		reader.attr("width", xTiles)
			  .attr("height", yTiles)
			  .attr("tilewidth", tileWidth)
			  .attr("tileheight", tileHeight);

		myMap = new YCTiledMap(mapId, xTiles, yTiles, tileWidth, tileHeight);

		reader.foreach("tileset", YCXmlReader::YCForEachWrapper<YCTmxHelper>(this, &YCTmxHelper::loadTileSet))
			  .foreach("layer", YCXmlReader::YCForEachWrapper<YCTmxHelper>(this, &YCTmxHelper::loadLayer));

		return myMap;
	}

private:

	//
	// 函数：loadTileSet(YCXmlReader& reader)
	//
	// 目的：读取图块
	//
	bool loadTileSet(YCXmlReader& reader)
	{
		int firstgid;
		char* name = NULL;
		int tileWidth;
		int tileHeight;

		reader.attr("firstgid", firstgid)
			  .attr("name", &name)
			  .attr("tilewidth", tileWidth)
			  .attr("tileheight", tileHeight);

		char* imageFile = NULL;
		int imageWidth;
		int imageHeight;

		reader.enter("image")
			    .attr("source", &imageFile)
				.attr("width", imageWidth)
				.attr("height", imageHeight)
			  .leave();

		//std::string imageFullPath = myPath + "\\" + imageFile;
		YCTileSet* tileSet = new YCTileSet(myMap, firstgid, name, imageFile, imageWidth, imageHeight);

		reader.foreach("tile", YCXmlReader::YCForEachWrapper1<YCTmxHelper, YCTileSet>(this, &YCTmxHelper::loadTile, tileSet));

		myMap->addTileSet(tileSet);

		return true;
	}

	struct Tile_Property_T
	{
		Tile_Property_T(YCTileSet* ts) : tileSet(ts) {}

		int tid;
		YCTileSet* tileSet;
	};

	//
	// 函数：loadTile(YCXmlReader& reader, YCTileSet* tileSet)
	//
	// 目的：读取图块中每块数据
	//
	bool loadTile(YCXmlReader& reader, YCTileSet* tileSet)
	{
		Tile_Property_T lpt(tileSet);

		reader.attr("id", lpt.tid)
			  .enter("properties")
			    .foreach("property", YCXmlReader::YCForEachWrapper1<YCTmxHelper, Tile_Property_T>(this, &YCTmxHelper::loadTileProperty, &lpt))
			  .leave();

		return true;
	}

	//
	// 函数：
	//
	// 目的：
	//
	bool loadTileProperty(YCXmlReader& reader, Tile_Property_T* lpt)
	{
		char* name = NULL;
		char* value = NULL;

		reader.attr("name", &name)
		      .attr("value", &value);

		lpt->tileSet->setProperty(lpt->tid, name, value);

		SAFE_DELETE(name);
		SAFE_DELETE(value);

		return true;
	}

	//
	// 函数：loadLayer(YCXmlReader& reader)
	//
	// 目的：读取层
	//
	bool loadLayer(YCXmlReader& reader)
	{
		char* name;
		int xTiles;
		int yTiles;

		reader.attr("name", &name)
			  .attr("width", xTiles)
			  .attr("height", yTiles);

		YCTiledLayer* layer = new YCTiledLayer(name, xTiles, yTiles);

		Layer_Gid_T lgt(layer);

		try 
		{
			reader.enter("properties")
					.foreach("property", YCXmlReader::YCForEachWrapper1<YCTmxHelper, YCTiledLayer>(this, &YCTmxHelper::loadLayerProperty, layer))
				.leave();
		}
		catch (YCException& e)
		{
			UNREFERENCED_PARAMETER(e);
			LOG_WARNING("YCTmxHelper::loadLayer加载layer" << name << " properties失败，或者没有设置！");
		}

		reader.enter("data")
			     .foreach("tile", YCXmlReader::YCForEachWrapper1<YCTmxHelper, Layer_Gid_T>(this, &YCTmxHelper::loadGID, &lgt))
			  .leave();

		myMap->addLayer(layer);

		return true;
	}

	struct Layer_Gid_T
	{
		Layer_Gid_T(YCTiledLayer* l) : layer(l), index(0) {}

		int index;
		YCTiledLayer* layer;
	};

	//
	// 函数：
	//
	// 目的：
	//
	bool loadLayerProperty(YCXmlReader& reader, YCTiledLayer* layer)
	{
		char* name = NULL;
		char* value = NULL;

		reader.attr("name", &name)
			  .attr("value", &value);

		layer->setProperty(name, value);

		SAFE_DELETE(name);
		SAFE_DELETE(value);

		return true;
	}

	//
	// 函数：
	//
	// 目的：
	//
	bool loadGID(YCXmlReader& reader, Layer_Gid_T* lgt)
	{
		int gid;

		reader.attr("gid", gid);
		lgt->layer->bindGID(lgt->index++, gid);

		return true;
	}

private:

	std::string myPath;

	YCTiledMap* myMap;
};

//
// 函数：build(const char* filename)
//
// 目的：构造地图对象
//
YCTiledMap* YCMapBuilder::build(unsigned int mapId, const char* filename)
{
	return YCTmxHelper().parse(mapId, filename);
}

