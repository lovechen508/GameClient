#include "YCIMapObject.h"

#include "YCBasic\YCException.h"

YCIMapObject::YCIMapObject()
{
}

YCIMapObject::~YCIMapObject()
{
}

//
// ������setLayer()
//
// Ŀ�ģ������������ڲ�
//
void YCIMapObject::setLayer(unsigned int layer)
{
	myLayer = layer;
}

//
// ������layer()
//
// Ŀ�ģ������������ڲ�
//
unsigned int YCIMapObject::getLayer() const
{
	return myLayer;
}

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
int YCIMapObject::compareLayer(const YCIMapObject* other) const
{
	if (other == NULL)
	{
		throw YCException(2002, "YCIMapObject::compareLayer otherΪNULL");
	}

	if      (other->getLayer() <  myLayer) return 1;
	else if (other->getLayer() >  myLayer) return -1;
	else return extraCompare(other);
}

///////////////////////////////////////////////////
// Private Part
///////////////////////////////////////////////////
	
//
// ������extraCompare(const YCIMapObject* other) const
//
// Ŀ�ģ����Ӳ�αȽϹ�ϵ
//
int YCIMapObject::extraCompare(const YCIMapObject* other) const
{
	return 0; // Ĭ���޸��ӱȽ�
}