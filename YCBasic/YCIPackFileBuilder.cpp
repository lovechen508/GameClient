#include "YCIPackFileBuilder.h"

#include "YCAssert.h"

YCIPackFileBuilder::YCIPackFileBuilder(unsigned int version)
	: myVersion(version)
{
	SMART_ASSERT(version > 0);
}


YCIPackFileBuilder::~YCIPackFileBuilder(void)
{
}

//
// 注释：取得Builder所处理的文件版本
//
unsigned int YCIPackFileBuilder::version() const
{
	return myVersion;
}
