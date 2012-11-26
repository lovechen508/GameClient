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
	// ������compressDirectory(const char* dir, const char *outputFilename)
	//
	// Ŀ�ģ���ָ��Ŀ¼ѹ����ָ���ļ������ڷ���
	//
	virtual bool compressDirectory(const char* dir, const char *outputFilename);

	//
	// ������bool extractMemory(const char* buf, int len, const char* targetPath);
	//
	// Ŀ�ģ���ѹָ��ѹ���ļ��ڴ�ӳ��targetPathĿ¼
	//
	virtual bool extractMemory(const char* buf, int len, const char* targetPath);

	//
	// ������bool extractFile(const char* filename, const char* targetPath);
	//
	// Ŀ�ģ���ѹָ��ѹ���ļ���targetPathĿ¼
	//
	virtual bool extractFile(const char* filename, const char* targetPath);
};

#endif

