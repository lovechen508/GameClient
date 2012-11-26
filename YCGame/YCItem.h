#ifndef _INCLUDE_YCITEM_H_
#define _INCLUDE_YCITEM_H_

#include "YCGraphic\YCISprite.h"

class YCIPackage;

/*
 * YCItem : ��Ʒ��
 *
 * ˵����
 *
 */
class YCItem :
	public YCISprite
{
public:

	YCItem(void);

	virtual ~YCItem(void);

	//
	// ������add2Package(YCIPackage* package)
	//
	// Ŀ�ģ�������Ʒ���ڰ���
	//
	void addToPackage(YCIPackage* package);
	void removeFromPackage();

	//
	// ������setIndex(unsigned int index)
	//       getIndex() const
	//
	// Ŀ�ģ����ã���ȡ��Ʒ�ڰ����е�λ��
	//
	void setIndex(unsigned int index);
	unsigned int getIndex() const;

private:

	//
	// ���ԣ���Ʒ���ڰ������ڵ���ʱΪNULL
	//
	YCIPackage* myPackage;

	//
	// ���ԣ��ڰ����е�λ�ñ��
	//
	unsigned int myIndex;

};

#endif

