#include "YCIFilePack.h"

#include <stdio.h>

#include "YCDList.h"
#include "YCLogger.h"
#include "YCAssert.h"
#include "YCIPackPersist.h"
#include "YCPackException.h"

YCIFilePack::YCIFilePack(const char* name, FILE* fd, YCIPackFileBuilder* builder)
	: YCIFileDirectory(this, name, YCIAbstractFile::FILE_PACK)
	, myBuilder(builder)
	, myIdleList(new YCDList())
{
	bind(fd);
}

YCIFilePack::~YCIFilePack(void)
{
	if (!fclose(myFd))
	{
		LOG_ERROR("YCIPackFile析构关闭文件失败：" << filename());
	}
	
	myIdleList->clear(YCDListClearHelper<IDLE_BLOCK>);
	SAFE_DELETE(myIdleList);
}

//
// 取得文件在包文件的偏移量
//
unsigned int YCIFilePack::getOffset()
{
	throw YCPackException(1002, "YCIFilePack::getOffset包文件不允许查找偏移量");
}

//
// 设置读写普通文件加解密，加解压对象
//
YCIPackFileBuilder* YCIFilePack::getBuilder()
{
	return myBuilder;
}

//////////////////////////////////////////////////////////////////////////
// Protected Part
//////////////////////////////////////////////////////////////////////////



