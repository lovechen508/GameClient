#ifndef _INCLUDE_YCIPACKAGE_H_
#define _INCLUDE_YCIPACKAGE_H_

#include "YCGraphic\YCIModel.h"

class YCItem;
class YCDList;

/*
 * YCIPackage : ��Ʒ�����ӿ���
 *
 * ˵����
 *
 *
 */
class YCIPackage :
	public YCIModel
{
public:

	YCIPackage(void);

	virtual ~YCIPackage(void);

	//
	// ������setCapacity/getCapacity
	//
	// Ŀ�ģ����ã���ȡ����
	// 
	void setCapacity(unsigned int n);
	unsigned int getCapacity() const;

	//
	// ������findById(unsigned long long id)
	//
	// Ŀ�ģ�����ID������Ʒ
	//
	YCItem * findById(unsigned long long id);

	//
	// ������setItemAt(unsigned int index, YCItem* item)
	//
	// Ŀ�ģ������Ʒ��Package��
	//
	void setItemAt(unsigned int index, YCItem* item);
	
	//
	// ������getItemAt(unsigned int index) const
	//
	// Ŀ�ģ���ȡĳλ�õ���Ʒ
	//
	YCItem* getItemAt(unsigned int index);

	//
	// ������removeItemAt(unsigned int index)
	//
	// Ŀ�ģ���ĳλ���Ƴ���Ʒ
	//
	YCItem* removeItemAt(unsigned int index);

	//
	// ������clear()
	//
	// Ŀ�ģ����
	//
	void clear();

	//
	// ������
	//
	// Ŀ�ģ�
	//

private:

	//
	// ����������
	//
	unsigned int myCapacity;

	//
	// ��������װ��Ʒ
	//
	YCDList *myItems;
};

#endif