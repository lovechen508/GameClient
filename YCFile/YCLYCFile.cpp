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
// 设置自身Header
//
void YCLYCFile::setSelfHeader(LYC_FILE_T_1* header)
{
	SMART_ASSERT(header != NULL);
	mySelfHeader = header;
}

//
// 在有了改动之后，强制刷新到磁盘
//
void YCLYCFile::flush()
{
	// 刷文件头
	if (isDirty())
	{
		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFile::flush Builder为空");
		}
		theBuilder->writeNormal(this);

		myDirty = false;
	}
}

//
// 注释：返回文件内容指针
//
const char* YCLYCFile::getContent()
{
	return myContent;
}

//
// 取得文件在包文件的偏移量
//
unsigned int YCLYCFile::getOffset()
{
	if (mySelfHeader->offset == 0)
	{
		throw YCPackException(1002, "YCLYCFile::getOffset尚未初始化");
	}
	return mySelfHeader->offset;
}

//
// 导出自身文件
//
void YCLYCFile::dump(const char* targetPath, bool overwrite)
{
	char theFullname[MAX_PATH] = {0};
	sprintf(theFullname, "%s\\%s", targetPath, filename());

	
	if (YCFileUtil::IsFileExist(theFullname))
	{
		if (YCFileUtil::IsDir(theFullname))
		{
			throw YCPackException(1002, "YCLYCFile::dump导出文件目标以文件夹存在");
		}

		if (!overwrite)
		{
			throw YCPackException(1002, "YCLYCFile::dump文件已存在");
		}
	}

	FILE* fd = fopen(theFullname, "wb");
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFile::dump覆盖文件失败");
	}

	if (myLength > 0)
	{
		SMART_ASSERT(myContent != NULL);
		const char* content = getContent();
		YCLYCFileBuilder1::write(fd, 0, myLength, content);
	}

	if (0 != fflush(fd))
	{
		throw YCPackException(1002, "YCLYCFile::dump刷新文件到磁盘失败");
	}
}
