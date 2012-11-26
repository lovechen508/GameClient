#ifndef _INCLUDE_YCTILEDLAYER_H_
#define _INCLUDE_YCTILEDLAYER_H_

#include <map>
#include <string>

class YCTiledMap;

/*
 * YCTiledLayer：Tiled层数据抽象类
 *
 * 注释：
 *          <layer name="background" width="32" height="60">
 *              <data>
 *                   <tile gid="1"/>
 *                   <tile gid="2"/>
 *                   ...
 *              </data>
 *          </layer>
 *
 */
class YCTiledLayer
{
public:

	typedef std::map<std::string, std::string> PROPERTYMAP;

	//
	// 说明：阻挡层，可选
	//
	static const char* BLOCK_LAYER_NAME;

	//
	// 说明：上层地图物件层，可选
	//
	static const char* LEVEL_2_MAP_OBJECT;

	//
	// 说明：上层地图小地表，可选
	//
	static const char* LEVEL_2_MAP_SMALL_GROUND;

	//
	// 说明：上层地图大地表
	//
	static const char* LEVEL_2_MAP_BIG_GROUND;

	//
	// 说明：下层地图，可选
	//
	static const char* LEVEL_1_MAP;
	
public:
	
	YCTiledLayer(const char* name,
		         unsigned int xTiles,
				 unsigned int yTiles);

	virtual ~YCTiledLayer(void);

	//
	// 函数：getLayerValue() const
	//
	// 目的：取得层的位置值
	//
	int getLayerValue() const;

	//
	// 函数：setProperty(const char* name, const char* value)
	//
	// 目的：设置Layer属性
	//
	void setProperty(const char* name, const char* value);

	//
	// 函数：hasProperty(const char* name)
	//
	// 目的：测试属性是否存在
	//
	bool hasProperty(const char* name);

	//
	// 函数：getProperty(const char* name)
	//
	// 目的：获取属性
	//
	const char* getProperty(const char* name);

	//
	// 函数：bindToMap(YCTiledMap* map)
	//
	// 目的：绑定到地图
	//
	void bindToMap(YCTiledMap* map);

	//
	// 函数：bindGID(unsigned int index, unsigned int gid)
	//
	// 目的：绑定地图块属性
	//
	void bindGID(unsigned int index, unsigned int gid);

	//
	// 函数：getGID(unsigned int x, unsigned int y)
	//
	// 目的：取得地图块对应Tileset的GID
	//
	unsigned int getGID(unsigned int x, unsigned int y);

	//
	// 函数：isDynamicZOrder()
	//
	// 目的：是否动态层
	//
	bool isDynamicZOrder();

	//
	// 函数：ZOrder()
	//
	// 目的：取得ZOrder
	//
	unsigned int ZOrder();
	
private:

	//
	// 属性：myProperites
	//
	// 目的：Layer属性集
	//
	PROPERTYMAP myProperties;

	//
	// 属性：myTiledMap
	//
	// 目的：所属地图
	//
	YCTiledMap* myTiledMap;

	//
	// 属性：myName
	//
	// 目的：层名称
	//
	const char* myName;

	//
	// 属性：myLayerValue
	//
	// 目的：取得层的值
	//
	int myLayerValue;

	//
	// 属性：myZOrder
	//
	// 目的：Z order, 1在最底层
	//       0为动态层
	//
	unsigned int myZOrder;

	//
	// 属性：myXTiles
	//
	// 目的：水平方向的地图块数
	//
	unsigned int myXTiles;

	//
	// 属性：myYTiles
	//
	// 目的：垂直方向的地图块数
	//
	unsigned int myYTiles;

	//
	// 属性：myGIDs
	//
	// 目的：GID二维数组一维数组存储方式
	//
	unsigned int* myGIDs;
	
};

#endif
