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
// YCTmxHelper Tmx�����ڲ���
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
			throw YCException(2002, "YCTiledMap::parse��Ч�ļ���");
		}

		YCXmlReader reader(mapFile);

		char mapPath[MAX_PATH] = {0};
		if (!YCFileUtil::GetFilePath(mapFile, mapPath, MAX_PATH))
		{
			throw YCException(2002, "YCTmxHelper::parse�޷�ȡ�õ�ͼ�ļ�����·����");
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
	// ������loadTileSet(YCXmlReader& reader)
	//
	// Ŀ�ģ���ȡͼ��
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
	// ������loadTile(YCXmlReader& reader, YCTileSet* tileSet)
	//
	// Ŀ�ģ���ȡͼ����ÿ������
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
	// ������
	//
	// Ŀ�ģ�
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
	// ������loadLayer(YCXmlReader& reader)
	//
	// Ŀ�ģ���ȡ��
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
			LOG_WARNING("YCTmxHelper::loadLayer����layer" << name << " propertiesʧ�ܣ�����û�����ã�");
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
	// ������
	//
	// Ŀ�ģ�
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
	// ������
	//
	// Ŀ�ģ�
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
// ������build(const char* filename)
//
// Ŀ�ģ������ͼ����
//
YCTiledMap* YCMapBuilder::build(unsigned int mapId, const char* filename)
{
	return YCTmxHelper().parse(mapId, filename);
}

