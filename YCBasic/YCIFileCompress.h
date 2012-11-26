#ifndef _INCLUDE_YCIFILECOMPRESS_H_
#define _INCLUDE_YCIFILECOMPRESS_H_

class YCAPI YCIFileCompress
{
public:
	YCIFileCompress(void);

	virtual ~YCIFileCompress(void);

	//
	// ������compressDirectory(const char* dir, const char *outputFilename)
	//
	// Ŀ�ģ���ָ��Ŀ¼ѹ����ָ���ļ������ڷ���
	//
	virtual bool compressDirectory(const char* dir, const char *outputFilename) = 0;

	//
	// ������bool extractMemory(const char* buf, int len, const char* targetPath);
	//
	// Ŀ�ģ���ѹָ��ѹ���ļ��ڴ�ӳ��targetPathĿ¼
	//
	virtual bool extractMemory(const char* buf, int len, const char* targetPath) = 0;

	//
	// ������bool extractFile(const char* filename, const char* targetPath);
	//
	// Ŀ�ģ���ѹָ��ѹ���ļ���targetPathĿ¼
	//
	virtual bool extractFile(const char* filename, const char* targetPath) = 0;
};

#endif
