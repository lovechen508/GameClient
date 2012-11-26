#ifndef _INCLUDE_YCTILESET_H_
#define _INCLUDE_YCTILESET_H_

#include <map>
#include <string>

class YCITexture;
class YCTiledMap;

/*
 * YCTileSet : 地图图片属性类
 *
 * 注释：
 *         <tileset firstgid="1921" name="block" tilewidth="64" tileheight="32">
 *             <image source="../shared/block.png" width="66" height="67"/>
 *             <tile id="0">
 *                 <properties>
 *                     <property name="block" value="1"/>
 *                 </properties>
 *             </tile>
 *          </tileset>
 *
 */
class YCTileSet
{

public:

	typedef std::map<std::string, std::string> PROPERTYMAP;
	typedef std::map<unsigned int, PROPERTYMAP> TILEPROPERTYMAP;

public:

	YCTileSet(YCTiledMap* map,
		      unsigned int firstGid,
		      const char* name,
			  const char* imageFile,
			  unsigned int imageWidth,
			  unsigned int imageHeight);

	virtual ~YCTileSet(void);

	//
	// 函数：setProperty(unsigned int tid, const char* name, const char* value)
	//
	// 目的：设置地图块属性
	//
	void setProperty(unsigned int tid, const char* name, const char* value);

	//
	// 函数：getName() const
	//
	// 目的：Tileset块名称
	//
	const char* getName() const;

	//
	// 函数：getFirstGID() const
	//
	// 目的：第一块Tid
	//
	unsigned int getFirstGID() const;

	//
	// 函数：getLastGID() const
	//
	// 目的：末块Tid
	//
	unsigned int getLastGID() const;

	//
	// 函数：hasGID(unsigned int gid) const
	//
	// 目的：是否有该GID
	//
	bool hasGID(unsigned int gid) const;

	//
	// 函数：calculatePosition(unsigned int gid, unsigned int& left, unsigned int& top)
	//
	// 目的：计算该GID在图层中的位置
	//
	void calculatePosition(unsigned int gid, unsigned int& left, unsigned int& top);

	//
	// 函数：getImageFile() const
	//
	// 目的：图片文件名
	//
	const char* getImageFile() const;

	//
	// 函数：setTexture(YCITexture* texture)
	//
	// 目的：绑定图片文件的内存纹理
	//
	void setTexture(YCITexture* texture);

	//
	// 函数：getTexture(YCITexture* texture)
	//
	// 目的：获取图片文件的内存纹理
	//
	YCITexture* getTexture();

	//
	// 函数：getImageWidth() const
	//
	// 目的：图片宽
	//
	unsigned int getImageWidth() const;

	//
	// 函数：getImageHeight() const
	//
	// 目的：图片高
	//
	unsigned int getImageHeight() const;

	//
	// 函数：hasProperty(unsigned int gid) const
	//
	// 目的：该地图块是否有属性配置
	//
	bool hasProperty(unsigned int gid) const;

	//
	// 函数：getProperties(unsigned int gid) const
	//
	// 目的：取得该块配置的属性
	//
	const PROPERTYMAP* getProperties(unsigned int gid) const;

private:

	//
	// 属性：myMap
	//
	// 目的：所属地图
	//
	YCTiledMap* myMap;

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
	// 属性：myFirstGId
	//
	// 目的：GID起始值
	//
	unsigned int myFirstGId;

	//
	// 属性：myLastGId
	//
	// 目的：GID结束值
	//
	unsigned int myLastGId;

	//
	// 属性：myName
	//
	// 目的：块集名称
	//
	const char* myName;

	//
	// 属性：myImageFile
	//
	// 目的：图片文件
	//
	const char* myImageFile;

	//
	// 属性：myTexture
	//
	// 目的：图片纹理, 生命周期
	//
	YCITexture* myTexture;

	//
	// 属性：myImageWidth
	//
	// 目的：图片宽度
	//
	unsigned int myImageWidth;

	//
	// 属性：myImageHeight
	//
	// 目的：图片高度
	//
	unsigned int myImageHeight;

	//
	// 属性：myTileProperties
	//
	// 目的：块属性集合
	//
	TILEPROPERTYMAP myTileProperties;
};

#endif

