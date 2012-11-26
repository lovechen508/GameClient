#include "YCIFileDirectory.h"

#include "YCDList.h"

YCIFileDirectory::YCIFileDirectory(YCIFilePack* pkg, const char* name, int type)
	: YCIAbstractFile(name, type)
	, myPack(pkg)
	, myParent(NULL)
{
}


YCIFileDirectory::~YCIFileDirectory(void)
{
}

//
// 取得文件所在包
//
YCIFilePack* YCIFileDirectory::getPack()
{
	return myPack;
}

//
// 设置父目录，有可能是YCIPackFile
//
void YCIFileDirectory::setParent(YCIFileDirectory* parent)
{
	myParent = parent;
}

//
// 获取父目录
//
YCIFileDirectory* YCIFileDirectory::getParent() const
{
	return myParent;
}

//////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////

