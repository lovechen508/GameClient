#include "YCZlibFile.h"


YCZlibFile::YCZlibFile(void)
{
}


YCZlibFile::~YCZlibFile(void)
{
}

//
// ������compressDirectory(const char* dir, const char *outputFilename)
//
// Ŀ�ģ���ָ��Ŀ¼ѹ����ָ���ļ������ڷ���
//
bool YCZlibFile::compressDirectory(const char* dir, const char *outputFilename)
{
	return true;
}

//
// ������bool extractMemory(const char* buf, int len, const char* targetPath);
//
// Ŀ�ģ���ѹָ��ѹ���ļ��ڴ�ӳ��targetPathĿ¼
//
bool YCZlibFile::extractMemory(const char* buf, int len, const char* targetPath)
{
	return true;
}

//
// ������bool extractFile(const char* filename, const char* targetPath);
//
// Ŀ�ģ���ѹָ��ѹ���ļ���targetPathĿ¼
//
bool YCZlibFile::extractFile(const char* filename, const char* targetPath)
{
	return true;
}
