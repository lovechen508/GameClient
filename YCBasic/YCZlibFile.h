#ifndef _INCLUDE_YCZLIBFILE_H_
#define _INCLUDE_YCZLIBFILE_H_

#include "YCIFileCompress.h"

class YCAPI YCZlibFile :
	public YCIFileCompress
{
public:
	
	YCZlibFile(void);
	
	virtual ~YCZlibFile(void);

	//
	// 函数：compressDirectory(const char* dir, const char *outputFilename)
	//
	// 目的：将指定目录压缩成指定文件，用于发布
	//
	virtual bool compressDirectory(const char* dir, const char *outputFilename);

	//
	// 函数：bool extractMemory(const char* buf, int len, const char* targetPath);
	//
	// 目的：解压指定压缩文件内存映像到targetPath目录
	//
	virtual bool extractMemory(const char* buf, int len, const char* targetPath);

	//
	// 函数：bool extractFile(const char* filename, const char* targetPath);
	//
	// 目的：解压指定压缩文件到targetPath目录
	//
	virtual bool extractFile(const char* filename, const char* targetPath);
};

#endif

