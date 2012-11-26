#include "YCZlibFile.h"


YCZlibFile::YCZlibFile(void)
{
}


YCZlibFile::~YCZlibFile(void)
{
}

//
// 函数：compressDirectory(const char* dir, const char *outputFilename)
//
// 目的：将指定目录压缩成指定文件，用于发布
//
bool YCZlibFile::compressDirectory(const char* dir, const char *outputFilename)
{
	return true;
}

//
// 函数：bool extractMemory(const char* buf, int len, const char* targetPath);
//
// 目的：解压指定压缩文件内存映像到targetPath目录
//
bool YCZlibFile::extractMemory(const char* buf, int len, const char* targetPath)
{
	return true;
}

//
// 函数：bool extractFile(const char* filename, const char* targetPath);
//
// 目的：解压指定压缩文件到targetPath目录
//
bool YCZlibFile::extractFile(const char* filename, const char* targetPath)
{
	return true;
}
