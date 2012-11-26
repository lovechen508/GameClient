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
// ���ø�Ŀ¼���п�����YCIPackFile
//
void YCIFileNormal::setParent(YCIFileDirectory* parent)
{
	myParent = parent;
}

//
// ��ȡ��Ŀ¼
//
YCIFileDirectory* YCIFileNormal::getParent() const
{
	return myParent;
}

//
// ȡ���ļ����ڰ�
//
YCIFilePack* YCIFileNormal::getPack()
{
	return myPack;
}

//
// �������ݣ����ⲿ�ļ�����ʱ
//
void YCIFileNormal::setContent(char* buf)
{
	myContent = buf;
}

//
// ע�ͣ������ļ�����
//
void YCIFileNormal::setLength(int len)
{
	myLength = len;
}

//
// ע�ͣ������ļ�����
//
int YCIFileNormal::getLength()
{
	return myLength;
}
