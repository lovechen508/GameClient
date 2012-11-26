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
// ȡ���ļ����ڰ�
//
YCIFilePack* YCIFileDirectory::getPack()
{
	return myPack;
}

//
// ���ø�Ŀ¼���п�����YCIPackFile
//
void YCIFileDirectory::setParent(YCIFileDirectory* parent)
{
	myParent = parent;
}

//
// ��ȡ��Ŀ¼
//
YCIFileDirectory* YCIFileDirectory::getParent() const
{
	return myParent;
}

//////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////

