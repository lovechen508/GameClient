#include "YCLYCFile.h"

#include "YCLYCDefine.h"
#include "YCLYCFileBuilder1.h"

#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCIFilePack.h"
#include "YCBasic\YCPackException.h"
#include "YCBasic\YCIPackFileBuilder.h"

YCLYCFile::YCLYCFile(YCIFilePack* pkg, const char* filename)
	: YCIFileNormal(pkg, filename)
	, mySelfHeader(NULL)
{
}


YCLYCFile::~YCLYCFile(void)
{
	
}

//
// ��������Header
//
void YCLYCFile::setSelfHeader(LYC_FILE_T_1* header)
{
	SMART_ASSERT(header != NULL);
	mySelfHeader = header;
}

//
// �����˸Ķ�֮��ǿ��ˢ�µ�����
//
void YCLYCFile::flush()
{
	// ˢ�ļ�ͷ
	if (isDirty())
	{
		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFile::flush BuilderΪ��");
		}
		theBuilder->writeNormal(this);

		myDirty = false;
	}
}

//
// ע�ͣ������ļ�����ָ��
//
const char* YCLYCFile::getContent()
{
	return myContent;
}

//
// ȡ���ļ��ڰ��ļ���ƫ����
//
unsigned int YCLYCFile::getOffset()
{
	if (mySelfHeader->offset == 0)
	{
		throw YCPackException(1002, "YCLYCFile::getOffset��δ��ʼ��");
	}
	return mySelfHeader->offset;
}

//
// ���������ļ�
//
void YCLYCFile::dump(const char* targetPath, bool overwrite)
{
	char theFullname[MAX_PATH] = {0};
	sprintf(theFullname, "%s\\%s", targetPath, filename());

	
	if (YCFileUtil::IsFileExist(theFullname))
	{
		if (YCFileUtil::IsDir(theFullname))
		{
			throw YCPackException(1002, "YCLYCFile::dump�����ļ�Ŀ�����ļ��д���");
		}

		if (!overwrite)
		{
			throw YCPackException(1002, "YCLYCFile::dump�ļ��Ѵ���");
		}
	}

	FILE* fd = fopen(theFullname, "wb");
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFile::dump�����ļ�ʧ��");
	}

	if (myLength > 0)
	{
		SMART_ASSERT(myContent != NULL);
		const char* content = getContent();
		YCLYCFileBuilder1::write(fd, 0, myLength, content);
	}

	if (0 != fflush(fd))
	{
		throw YCPackException(1002, "YCLYCFile::dumpˢ���ļ�������ʧ��");
	}
}
