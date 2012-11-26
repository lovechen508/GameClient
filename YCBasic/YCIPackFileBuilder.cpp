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
// ע�ͣ�ȡ��Builder��������ļ��汾
//
unsigned int YCIPackFileBuilder::version() const
{
	return myVersion;
}
