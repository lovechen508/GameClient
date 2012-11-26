#ifndef _INCLUDE_YCTILESET_H_
#define _INCLUDE_YCTILESET_H_

#include <map>
#include <string>

class YCITexture;
class YCTiledMap;

/*
 * YCTileSet : ��ͼͼƬ������
 *
 * ע�ͣ�
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
	// ������setProperty(unsigned int tid, const char* name, const char* value)
	//
	// Ŀ�ģ����õ�ͼ������
	//
	void setProperty(unsigned int tid, const char* name, const char* value);

	//
	// ������getName() const
	//
	// Ŀ�ģ�Tileset������
	//
	const char* getName() const;

	//
	// ������getFirstGID() const
	//
	// Ŀ�ģ���һ��Tid
	//
	unsigned int getFirstGID() const;

	//
	// ������getLastGID() const
	//
	// Ŀ�ģ�ĩ��Tid
	//
	unsigned int getLastGID() const;

	//
	// ������hasGID(unsigned int gid) const
	//
	// Ŀ�ģ��Ƿ��и�GID
	//
	bool hasGID(unsigned int gid) const;

	//
	// ������calculatePosition(unsigned int gid, unsigned int& left, unsigned int& top)
	//
	// Ŀ�ģ������GID��ͼ���е�λ��
	//
	void calculatePosition(unsigned int gid, unsigned int& left, unsigned int& top);

	//
	// ������getImageFile() const
	//
	// Ŀ�ģ�ͼƬ�ļ���
	//
	const char* getImageFile() const;

	//
	// ������setTexture(YCITexture* texture)
	//
	// Ŀ�ģ���ͼƬ�ļ����ڴ�����
	//
	void setTexture(YCITexture* texture);

	//
	// ������getTexture(YCITexture* texture)
	//
	// Ŀ�ģ���ȡͼƬ�ļ����ڴ�����
	//
	YCITexture* getTexture();

	//
	// ������getImageWidth() const
	//
	// Ŀ�ģ�ͼƬ��
	//
	unsigned int getImageWidth() const;

	//
	// ������getImageHeight() const
	//
	// Ŀ�ģ�ͼƬ��
	//
	unsigned int getImageHeight() const;

	//
	// ������hasProperty(unsigned int gid) const
	//
	// Ŀ�ģ��õ�ͼ���Ƿ�����������
	//
	bool hasProperty(unsigned int gid) const;

	//
	// ������getProperties(unsigned int gid) const
	//
	// Ŀ�ģ�ȡ�øÿ����õ�����
	//
	const PROPERTYMAP* getProperties(unsigned int gid) const;

private:

	//
	// ���ԣ�myMap
	//
	// Ŀ�ģ�������ͼ
	//
	YCTiledMap* myMap;

	//
	// ���ԣ�myXTiles
	//
	// Ŀ�ģ�ˮƽ����ĵ�ͼ����
	//
	unsigned int myXTiles;

	//
	// ���ԣ�myYTiles
	//
	// Ŀ�ģ���ֱ����ĵ�ͼ����
	//
	unsigned int myYTiles;

	//
	// ���ԣ�myFirstGId
	//
	// Ŀ�ģ�GID��ʼֵ
	//
	unsigned int myFirstGId;

	//
	// ���ԣ�myLastGId
	//
	// Ŀ�ģ�GID����ֵ
	//
	unsigned int myLastGId;

	//
	// ���ԣ�myName
	//
	// Ŀ�ģ��鼯����
	//
	const char* myName;

	//
	// ���ԣ�myImageFile
	//
	// Ŀ�ģ�ͼƬ�ļ�
	//
	const char* myImageFile;

	//
	// ���ԣ�myTexture
	//
	// Ŀ�ģ�ͼƬ����, ��������
	//
	YCITexture* myTexture;

	//
	// ���ԣ�myImageWidth
	//
	// Ŀ�ģ�ͼƬ���
	//
	unsigned int myImageWidth;

	//
	// ���ԣ�myImageHeight
	//
	// Ŀ�ģ�ͼƬ�߶�
	//
	unsigned int myImageHeight;

	//
	// ���ԣ�myTileProperties
	//
	// Ŀ�ģ������Լ���
	//
	TILEPROPERTYMAP myTileProperties;
};

#endif

