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
		LOG_ERROR("YCIPackFile�����ر��ļ�ʧ�ܣ�" << filename());
	}
	
	myIdleList->clear(YCDListClearHelper<IDLE_BLOCK>);
	SAFE_DELETE(myIdleList);
}

//
// ȡ���ļ��ڰ��ļ���ƫ����
//
unsigned int YCIFilePack::getOffset()
{
	throw YCPackException(1002, "YCIFilePack::getOffset���ļ����������ƫ����");
}

//
// ���ö�д��ͨ�ļ��ӽ��ܣ��ӽ�ѹ����
//
YCIPackFileBuilder* YCIFilePack::getBuilder()
{
	return myBuilder;
}

//////////////////////////////////////////////////////////////////////////
// Protected Part
//////////////////////////////////////////////////////////////////////////



