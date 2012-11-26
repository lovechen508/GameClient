#ifndef _INCLUDE_YCIMAPFACTORY_H_
#define _INCLUDE_YCIMAPFACTORY_H_

class YCIMap;

/*
 * YCIMapFactory : 
 *
 * ע�� ����ͼ�����������
 *
 *
 */
class YCAPI YCIMapFactory
{

public:

	YCIMapFactory(void) {}

	virtual ~YCIMapFactory(void) {}

	//
	// ������acquire(unsigned int mapId)
	//
	// Ŀ�ģ���ȡ��ͼ
	//
	virtual YCIMap* acquire(unsigned int mapId) = 0;
};

#endif

