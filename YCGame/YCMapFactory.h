#ifndef _INCLUDE_YCMAPFACTORY_H_
#define _INCLUDE_YCMAPFACTORY_H_

#include "YCGraphic\YCIMapFactory.h"

#include <map>

class YCTiledMap;
class YCConfig;

/*
 * YCMapFactory : 
 *
 * ע�� ����ͼ�����������
 *
 *
 */
class YCMapFactory : public YCIMapFactory
{

public:

	YCMapFactory(void);

	~YCMapFactory(void);

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ���ʼ����ͼ����
	//
	bool initialize(YCConfig& config);

	//
	// ������acquire(unsigned int mapId)
	//
	// Ŀ�ģ���ȡ��ͼ
	//
	virtual YCIMap* acquire(unsigned int mapId);

	//
	// ������release(YCMap* map)
	//
	// Ŀ�ģ��ͷŵ�ͼ
	//
	//virtual void release(YCIMap* map);

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷŵ�ͼ����
	//
	void finalize();

private:

	//
	// ���ԣ�myMapDirectory
	//
	// Ŀ�ģ���ͼ�ļ��洢·��
	//
	std::string myMapDirectory;

	//
	// ���ԣ�myMaps
	//
	// Ŀ�ģ���ͼ�ڴ澵��
	//
	std::map<unsigned int, YCTiledMap*> myMaps;

};

#endif

