#ifndef _INCLUDE_YCIMAPOBJECT_H_
#define _INCLUDE_YCIMAPOBJECT_H_

#include "YCISprite.h"

/*
 * YCIMapObject : �ɷ����ڵ�ͼ�϶���ӿ���
 *
 * ˵����
 *
 *
 *
 */
class YCAPI YCIMapObject : public YCISprite
{
public:

	YCIMapObject();

	virtual ~YCIMapObject();

	//
	// ������setLayer()
	//
	// Ŀ�ģ������������ڲ�
	//
	void setLayer(unsigned int layer);

	//
	// ������getLayer()
	//
	// Ŀ�ģ������������ڲ�
	//
	unsigned int getLayer() const;

	//
	// ������compareLayer(const YCIMapObject* other) const
	//
	// Ŀ�ģ��Ƚ��������໥��ι�ϵ
	//      
	// ����ֵ��
	//        1 : ������ǰ
	//        0 : ͬ��
	//       -1 �� other��ǰ
	//
	int compareLayer(const YCIMapObject* other) const;

private:

	//
	// ������extraCompare(const YCIMapObject* other) const
	//
	// Ŀ�ģ����Ӳ�αȽϹ�ϵ
	//
	virtual int extraCompare(const YCIMapObject* other) const;

protected:

	//
	// ���ԣ�myLayer
	//
	// Ŀ�ģ��������ڲ�
	//
	unsigned int myLayer;
};

#endif