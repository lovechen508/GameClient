#ifndef _INCLUDE_YCTILEDLAYER_H_
#define _INCLUDE_YCTILEDLAYER_H_

#include <map>
#include <string>

class YCTiledMap;

/*
 * YCTiledLayer��Tiled�����ݳ�����
 *
 * ע�ͣ�
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
	// ˵�����赲�㣬��ѡ
	//
	static const char* BLOCK_LAYER_NAME;

	//
	// ˵�����ϲ��ͼ����㣬��ѡ
	//
	static const char* LEVEL_2_MAP_OBJECT;

	//
	// ˵�����ϲ��ͼС�ر���ѡ
	//
	static const char* LEVEL_2_MAP_SMALL_GROUND;

	//
	// ˵�����ϲ��ͼ��ر�
	//
	static const char* LEVEL_2_MAP_BIG_GROUND;

	//
	// ˵�����²��ͼ����ѡ
	//
	static const char* LEVEL_1_MAP;
	
public:
	
	YCTiledLayer(const char* name,
		         unsigned int xTiles,
				 unsigned int yTiles);

	virtual ~YCTiledLayer(void);

	//
	// ������getLayerValue() const
	//
	// Ŀ�ģ�ȡ�ò��λ��ֵ
	//
	int getLayerValue() const;

	//
	// ������setProperty(const char* name, const char* value)
	//
	// Ŀ�ģ�����Layer����
	//
	void setProperty(const char* name, const char* value);

	//
	// ������hasProperty(const char* name)
	//
	// Ŀ�ģ����������Ƿ����
	//
	bool hasProperty(const char* name);

	//
	// ������getProperty(const char* name)
	//
	// Ŀ�ģ���ȡ����
	//
	const char* getProperty(const char* name);

	//
	// ������bindToMap(YCTiledMap* map)
	//
	// Ŀ�ģ��󶨵���ͼ
	//
	void bindToMap(YCTiledMap* map);

	//
	// ������bindGID(unsigned int index, unsigned int gid)
	//
	// Ŀ�ģ��󶨵�ͼ������
	//
	void bindGID(unsigned int index, unsigned int gid);

	//
	// ������getGID(unsigned int x, unsigned int y)
	//
	// Ŀ�ģ�ȡ�õ�ͼ���ӦTileset��GID
	//
	unsigned int getGID(unsigned int x, unsigned int y);

	//
	// ������isDynamicZOrder()
	//
	// Ŀ�ģ��Ƿ�̬��
	//
	bool isDynamicZOrder();

	//
	// ������ZOrder()
	//
	// Ŀ�ģ�ȡ��ZOrder
	//
	unsigned int ZOrder();
	
private:

	//
	// ���ԣ�myProperites
	//
	// Ŀ�ģ�Layer���Լ�
	//
	PROPERTYMAP myProperties;

	//
	// ���ԣ�myTiledMap
	//
	// Ŀ�ģ�������ͼ
	//
	YCTiledMap* myTiledMap;

	//
	// ���ԣ�myName
	//
	// Ŀ�ģ�������
	//
	const char* myName;

	//
	// ���ԣ�myLayerValue
	//
	// Ŀ�ģ�ȡ�ò��ֵ
	//
	int myLayerValue;

	//
	// ���ԣ�myZOrder
	//
	// Ŀ�ģ�Z order, 1����ײ�
	//       0Ϊ��̬��
	//
	unsigned int myZOrder;

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
	// ���ԣ�myGIDs
	//
	// Ŀ�ģ�GID��ά����һά����洢��ʽ
	//
	unsigned int* myGIDs;
	
};

#endif
