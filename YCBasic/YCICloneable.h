#ifndef _INCLUDE_YCICLONEABLE_H_
#define _INCLUDE_YCICLONEABLE_H_

/*
 * YCICloneable : �ɿ�¡�ӿ���
 */
template<typename T>
class YCICloneable
{
public:

	//
	// ������clone()
	//
	// Ŀ�ģ���¡��ǰ����
	//
	virtual T* clone() = 0;
};

#endif