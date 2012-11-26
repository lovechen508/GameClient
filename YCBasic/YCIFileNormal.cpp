#include "YCIFileNormal.h"

#include "YCDef.h"
#include "YCAssert.h"
#include "YCException.h"

YCIFileNormal::YCIFileNormal(YCIFilePack* pkg, const char* name)
	: YCIAbstractFile(name, YCIAbstractFile::FILE_NORMAL)
	, myPack(pkg)
	, myParent(NULL)
	, myContent(NULL)
{
}


YCIFileNormal::~YCIFileNormal(void)
{
	SAFE_DELETE(myContent);
}

//
// 设置父目录，有可能是YCIPackFile
//
void YCIFileNormal::setParent(YCIFileDirectory* parent)
{
	myParent = parent;
}

//
// 获取父目录
//
YCIFileDirectory* YCIFileNormal::getParent() const
{
	return myParent;
}

//
// 取得文件所在包
//
YCIFilePack* YCIFileNormal::getPack()
{
	return myPack;
}

//
// 设置数据，从外部文件创建时
//
void YCIFileNormal::setContent(char* buf)
{
	myContent = buf;
}

//
// 注释：设置文件长度
//
void YCIFileNormal::setLength(int len)
{
	myLength = len;
}

//
// 注释：返回文件长度
//
int YCIFileNormal::getLength()
{
	return myLength;
}
