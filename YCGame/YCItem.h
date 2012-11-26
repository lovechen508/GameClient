#ifndef _INCLUDE_YCITEM_H_
#define _INCLUDE_YCITEM_H_

#include "YCGraphic\YCISprite.h"

class YCIPackage;

/*
 * YCItem : 物品类
 *
 * 说明：
 *
 */
class YCItem :
	public YCISprite
{
public:

	YCItem(void);

	virtual ~YCItem(void);

	//
	// 函数：add2Package(YCIPackage* package)
	//
	// 目的：设置物品所在包裹
	//
	void addToPackage(YCIPackage* package);
	void removeFromPackage();

	//
	// 函数：setIndex(unsigned int index)
	//       getIndex() const
	//
	// 目的：设置，获取物品在包裹中的位置
	//
	void setIndex(unsigned int index);
	unsigned int getIndex() const;

private:

	//
	// 属性：物品所在包裹，在地上时为NULL
	//
	YCIPackage* myPackage;

	//
	// 属性：在包裹中的位置编号
	//
	unsigned int myIndex;

};

#endif

