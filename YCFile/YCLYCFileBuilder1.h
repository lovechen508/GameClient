#ifndef _INCLUDE_YCLYCFILEBUILDER_H_
#define _INCLUDE_YCLYCFILEBUILDER_H_

#include <string>

#include "YCBasic\YCDef.h"
#include "YCBasic\YCIPackFileBuilder.h"

class YCIPackPersist;

/*
 * YCLYCFileBuilder1 : Version 1 包文件创建器
 */
class YCLYCFileBuilder1 :
	public YCIPackFileBuilder
{
public:

	YCLYCFileBuilder1(void);
	
	virtual ~YCLYCFileBuilder1(void);

	//
	// 注释：则在指定位置创建包文件
	//

	///////////////////////////////////////////
	// 包文件操作
	///////////////////////////////////////////
	virtual YCIFilePack* createPack(const char* packFile);
	virtual YCIFilePack* loadPack(const char* packFile);
	virtual bool writePack(YCIFilePack* packFile);

	///////////////////////////////////////////
	// 文件夹操作
	///////////////////////////////////////////
	virtual YCIFileDirectory* createDir(YCIFileDirectory* parent, int slot, const char* dirName);
	virtual YCIFileDirectory* loadDir(YCIFileDirectory* parent, void * header);
	virtual bool writeDir(YCIFileDirectory* dir);

	///////////////////////////////////////////
	// 普通文件操作
	///////////////////////////////////////////
	virtual YCIFileNormal* createNormal(YCIFileDirectory* parent, int slot, const char* filename, const char* content, unsigned int length);
	virtual YCIFileNormal* createNormal(YCIFileDirectory* parent, int slot, const char* fullPath);
	virtual YCIFileNormal* loadNormal(YCIFileDirectory* parent, void * header);
	virtual bool writeNormal(YCIFileNormal* normal);

public:

	//
	// 加载文件段，客户自己分配内存
	//
	static bool read(FILE* fd, unsigned int offset, unsigned int length, char* out);

	//
	// 写入文件段
	//
	static bool write(FILE* fd, unsigned int offset, unsigned int length, const char* src);

private:

	YCIPackPersist* myPersist;
};

#endif