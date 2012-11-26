#ifndef _INCLUDE_YCIPACKFILEBUILDER_H_
#define _INCLUDE_YCIPACKFILEBUILDER_H_

class YCIFilePack;
class YCIFileNormal;
class YCIFileDirectory;

//
// YCIPackFileBuilder : 封装磁盘操作
//
// 说明：
//       每种文件类型都包含两类操作，
//
//       1. 不存在，从磁盘创建
//       2. 已存在，从磁盘读入
//       3. 已修改，往磁盘写入
//
class YCAPI YCIPackFileBuilder
{
public:

	YCIPackFileBuilder(unsigned int version);

	virtual ~YCIPackFileBuilder(void);

	//
	// 注释：则在指定位置创建包文件
	//

	///////////////////////////////////////////
	// 包文件操作
	///////////////////////////////////////////
	virtual YCIFilePack* createPack(const char* packFile) = 0;
	virtual YCIFilePack* loadPack(const char* packFile) = 0;
	virtual bool writePack(YCIFilePack* packFile) = 0;

	///////////////////////////////////////////
	// 文件夹操作
	///////////////////////////////////////////
	virtual YCIFileDirectory* createDir(YCIFileDirectory* parent, int slot, const char* dirName) = 0;
	virtual YCIFileDirectory* loadDir(YCIFileDirectory* parent, void * header) = 0;
	virtual bool writeDir(YCIFileDirectory* dir) = 0;

	///////////////////////////////////////////
	// 普通文件操作
	///////////////////////////////////////////
	virtual YCIFileNormal* createNormal(YCIFileDirectory* parent, int slot, const char* filename, const char* content, unsigned int length) = 0;
	virtual YCIFileNormal* createNormal(YCIFileDirectory* parent, int slot, const char* fullPath) = 0;
	virtual YCIFileNormal* loadNormal(YCIFileDirectory* parent, void * header) = 0;
	virtual bool writeNormal(YCIFileNormal* normal) = 0;

	//
	// 注释：取得Builder所处理的文件版本
	//
	unsigned int version() const;

private:

	//
	// Builder对应的文件版本
	//
	const unsigned int myVersion;

};

#endif

