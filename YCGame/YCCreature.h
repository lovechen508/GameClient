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
	// ���������飺
	//
	std::string myName;      // ����

	//
	// ��ۻ��������飺
	//
	unsigned char myDirection;

};

#endif

