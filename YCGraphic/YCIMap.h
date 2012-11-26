#ifndef _INCLUDE_YCIMAP_H_
#define _INCLUDE_YCIMAP_H_

class YCIMapTile;
class YCIMapObject;

#include "YCIModel.h"

/*
 * YCIMap : ��ͼ����ӿ���
 *
 * ˵����
 *
 *
 *
 */
class YCAPI YCIMap :
	public YCIModel
{

public:

	YCIMap(unsigned int mapId,
		   unsigned int xTiles,
		   unsigned int yTiles);

	virtual ~YCIMap(void);
		
	//
	// ������initialize()
	//
	// Ŀ�ģ�����ģ�ͳ�ʼ��
	//
	virtual bool initialize() = 0;
	
	//
	// ������finialize()
	//
	// Ŀ�ģ��ͷ�����ģ���е���Դ
	//
	virtual void finialize() = 0;	

	//
	// ������id()
	//
	// Ŀ�ģ���ͼID
	//
	unsigned int id() const;

	//
	// ������xTiles() const
	//
	// Ŀ�ģ�X�����ͼ����
	//
	unsigned int xTiles() const;

	//
	// ������yTiles() const
	//
	// Ŀ�ģ�Y�����ͼ����
	//
	unsigned int yTiles() const;

	//
	// ������setTileAt(YCIMapTile* mapTile, unsigned int x, unsigned int y)
	//
	// Ŀ�ģ���������λ��(x, y)�ĵ�ͼ��
	//
	void setTileAt(YCIMapTile* mapTile, unsigned int x, unsigned int y);

	//
	// ������getTileAt(unsigned int x, unsigned int y)
	//
	// Ŀ�ģ�ȡ������λ��(x, y)�ĵ�ͼ��
	//
	YCIMapTile* getTileAt(unsigned int x, unsigned int y);

	//
	// ������createTiles()
	//
	// Ŀ�ģ�������ͼ
	//
	bool createTiles();

	//
	// ������destoryTiles()
	//
	// Ŀ�ģ��ͷŵ�ͼ��
	//
	void destoryTiles();
	
	//
	// ������setBusy(bool busy)
	//
	// Ŀ�ģ����õ�ͼ�Ƿ�ʹ����
	//
	//void setBusy(bool busy);

	//
	// ������isBusy()
	//
	// Ŀ�ģ�����ͼ�Ƿ�ʹ����
	//
	//bool isBusy();

protected:
	
	//
	// ���ԣ���ͼID
	//
	unsigned int myId;

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

private:
	
	//
	// ���ԣ���ͼʹ���б�־
	//
	//bool myBusyFlag;

	//
	// ���ԣ�myMapArray
	//
	// Ŀ�ģ�
	//
	YCIMapTile** myMapArray;
};

#endif

