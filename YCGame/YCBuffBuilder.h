#ifndef _INCLUDE_YCBUFFBUILDER_H_
#define _INCLUDE_YCBUFFBUILDER_H_

class YCDList;

struct YCBuffBuilder
{
	//
	// ����YCBuffMeta����
	//
	YCDList* build(const char* filename);
};

#endif

