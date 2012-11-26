#ifndef _INCLUDE_YCCREATURE_H_
#define _INCLUDE_YCCREATURE_H_

#include <string>

#include "YCMapObject.h"

class YCCreature 
	: public YCMapObject
{
public:
	
	YCCreature(void);
	
	virtual ~YCCreature(void);

private:

	//
	// 基本属性组：
	//
	std::string myName;      // 名称

	//
	// 外观基本属性组：
	//
	unsigned char myDirection;

};

#endif

