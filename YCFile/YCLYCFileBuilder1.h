#ifndef _INCLUDE_YCLYCFILEBUILDER_H_
#define _INCLUDE_YCLYCFILEBUILDER_H_

#include <string>

#include "YCBasic\YCDef.h"
#include "YCBasic\YCIPackFileBuilder.h"

class YCIPackPersist;

/*
 * YCLYCFileBuilder1 : Version 1 ���ļ�������
 */
class YCLYCFileBuilder1 :
	public YCIPackFileBuilder
{
public:

	YCLYCFileBuilder1(void);
	
	virtual ~YCLYCFileBuilder1(void);

	//
	// ע�ͣ�����ָ��λ�ô������ļ�
	//

	///////////////////////////////////////////
	// ���ļ�����
	///////////////////////////////////////////
	virtual YCIFilePack* createPack(const char* packFile);
	virtual YCIFilePack* loadPack(const char* packFile);
	virtual bool writePack(YCIFilePack* packFile);

	///////////////////////////////////////////
	// �ļ��в���
	///////////////////////////////////////////
	virtual YCIFileDirectory* createDir(YCIFileDirectory* parent, int slot, const char* dirName);
	virtual YCIFileDirectory* loadDir(YCIFileDirectory* parent, void * header);
	virtual bool writeDir(YCIFileDirectory* dir);

	///////////////////////////////////////////
	// ��ͨ�ļ�����
	///////////////////////////////////////////
	virtual YCIFileNormal* createNormal(YCIFileDirectory* parent, int slot, const char* filename, const char* content, unsigned int length);
	virtual YCIFileNormal* createNormal(YCIFileDirectory* parent, int slot, const char* fullPath);
	virtual YCIFileNormal* loadNormal(YCIFileDirectory* parent, void * header);
	virtual bool writeNormal(YCIFileNormal* normal);

public:

	//
	// �����ļ��Σ��ͻ��Լ������ڴ�
	//
	static bool read(FILE* fd, unsigned int offset, unsigned int length, char* out);

	//
	// д���ļ���
	//
	static bool write(FILE* fd, unsigned int offset, unsigned int length, const char* src);

private:

	YCIPackPersist* myPersist;
};

#endif