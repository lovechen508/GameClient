#ifndef _INCLUDE_YCMAPOBJECT_H_
#define _INCLUDE_YCMAPOBJECT_H_

#include "YCGraphic\YCIMapObject.h"

/*
 * YCMapObject: ��ͼ����
 *
 * ˵���������ڵ�ͼ�ϳ��ֵĶ���
 *
 *
 */
class YCMapObject
	: public YCIMapObject
{
public:

	YCMapObject(void);

	virtual ~YCMapObject(void);

	//
	// ������draw()
	//
	// Ŀ�ģ��ڵ�ͼ���������Ļ���
	//
	virtual void draw();

};

#endif

