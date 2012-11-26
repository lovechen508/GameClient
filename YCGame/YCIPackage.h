#ifndef _INCLUDE_YCIPACKAGE_H_
#define _INCLUDE_YCIPACKAGE_H_

#include "YCGraphic\YCIModel.h"

class YCItem;
class YCDList;

/*
 * YCIPackage : 物品容器接口类
 *
 * 说明：
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
	// 函数：setCapacity/getCapacity
	//
	// 目的：设置，获取容量
	// 
	void setCapacity(unsigned int n);
	unsigned int getCapacity() const;

	//
	// 函数：findById(unsigned long long id)
	//
	// 目的：根据ID查找物品
	//
	YCItem * findById(unsigned long long id);

	//
	// 函数：setItemAt(unsigned int index, YCItem* item)
	//
	// 目的：添加物品到Package中
	//
	void setItemAt(unsigned int index, YCItem* item);
	
	//
	// 函数：getItemAt(unsigned int index) const
	//
	// 目的：获取某位置的物品
	//
	YCItem* getItemAt(unsigned int index);

	//
	// 函数：removeItemAt(unsigned int index)
	//
	// 目的：从某位置移除物品
	//
	YCItem* removeItemAt(unsigned int index);

	//
	// 函数：clear()
	//
	// 目的：清空
	//
	void clear();

	//
	// 函数：
	//
	// 目的：
	//

private:

	//
	// 函数：容量
	//
	unsigned int myCapacity;

	//
	// 函数：所装物品
	//
	YCDList *myItems;
};

#endif