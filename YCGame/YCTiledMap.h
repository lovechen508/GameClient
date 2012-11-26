#ifndef _INCLUDE_YCTILEDMAP_H_
#define _INCLUDE_YCTILEDMAP_H_

#include "YCGraphic\YCIMap.h"

class YCDList;
class YCTileSet;
class YCTiledLayer;

/*
 * YCTiledMap : TiledMap �������ɵĵ�ͼ��
 *
 * ע�ͣ�
 *	    <map version="1.0" orientation="orthogonal" width="32" height="60" tilewidth="64" tileheight="32">
 *         <tileset firstgid="1" name="background" tilewidth="64" tileheight="32">
 *             <image source="ice2.png" width="2048" height="1920"/>
 *         </tileset>
 *         <tileset firstgid="1921" name="block" tilewidth="64" tileheight="32">
 *             <image source="../shared/block.png" width="66" height="67"/>
 *             <tile id="0">
 *                 <properties>
 *                     <property name="block" value="1"/>
 *                 </properties>
 *             </tile>
 *          </tileset>
 *          <layer name="background" width="32" height="60">
 *              <data>
 *                   <tile gid="1"/>
 *                   <tile gid="2"/>
 *                   ...
 *              </data>
 *          </layer>
 *          <layer name="block" width="32" height="60">
 *              <data>
 *                    <tile gid="0"/>
 *                    <tile gid="1921" />
 *              </data>
 *          </layer>
 *      </map>
 */
class YCTiledMap :
	public YCIMap
{

public:

	YCTiledMap(unsigned int mapId,
		       unsigned int xTiles,
		       unsigned int yTiles,
			   unsigned int tileWidth,
			   unsigned int tileHeight);

	virtual ~YCTiledMap(void);

	//
	// ������initialize()
	//
	// Ŀ�ģ�����ģ�ͳ�ʼ��
	//
	virtual bool initialize();

	//
	// ������addTileSet(YCTileSet* tileSet)
	//
	// Ŀ�ģ�����ͼ��
	//
	void addTileSet(YCTileSet* tileSet);

	//
	// ������addLayer(YCTiledLayer* layer)
	//
	// Ŀ�ģ����Ӳ�
	//
	void addLayer(YCTiledLayer* layer);

	//
	// ������getTileWidth()
	//
	// Ŀ�ģ�ȡ�õ�ͼ����
	//
	unsigned int getTileWidth();

	//
	// ������getTileHeight()
	//
	// Ŀ�ģ�ȡ�õ�ͼ��߶�
	//
	unsigned int getTileHeight();

	//
	// ������finialize()
	//
	// Ŀ�ģ��ͷ�����ģ���е���Դ
	//
	virtual void finialize();

private:

	//
	// ���ԣ�myTilesets
	//
	// Ŀ�ģ�ͼ���б�
	//
	YCDList* myTilesets;

	//
	// ���ԣ�myMapLayers
	//
	// Ŀ�ģ����б�zorder����
	//      Խ�ӽ�����ǰ�ˣ���Խ��
	//
	YCDList* myMapLayers;

	//
	// ���ԣ�myTileWidth
	//
	// Ŀ�ģ���ͼ����
	//
	unsigned int myTileWidth;

	//
	// ���ԣ�myTileHeight
	//
	// Ŀ�ģ���ͼ��߶�
	//
	unsigned int myTileHeight;

};

#endif

