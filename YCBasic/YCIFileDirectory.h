#ifndef _INCLUDEYCIFILEDIRECTORY_H_
#define _INCLUDEYCIFILEDIRECTORY_H_

#include "YCIAbstractFile.h"


class YCDList;
class YCIFilePack;
class YCIFileNormal;

class YCAPI YCIFileDirectory : public YCIAbstractFile
{
public:

	YCIFileDirectory(YCIFilePack* pkg, const char* name, int type=YCIAbstractFile::FILE_DIR);

	virtual ~YCIFileDirectory(void);

	//
	// 取得文件所在包
	//
	YCIFilePack* getPack();

	//
	// 设置父目录，有可能是YCIPackFile
	//
	void setParent(YCIFileDirectory* parent);

	//
	// 获取父目录
	//
	YCIFileDirectory* getParent() const;

	//
	// 添加磁盘reguleFile到目录中
	//
	virtual bool add(const char* reguleFile, bool overwrite) = 0;

	//
	// 添加内存文件到目录中
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool overwrite) = 0;

	//
	// 添加子目录，是否递归
	//
	virtual bool mkdir(const char* dir, bool recursive) = 0;

	//
	// 是否包含fileInPack, 存在返回文件类型，否则返回0
	//
	virtual int has(const char* fileInPack) = 0;

	//
	// 抽取packFile文件
	//
	virtual YCIAbstractFile* get(const char* packFile) = 0;

	//
	// 删除packFile文件，找到置标志位即可
	//
	virtual bool remove(const char* packFile) = 0;

	//
	// 删除packFile文件，找到置标志位即可
	//
	virtual bool remove(YCIAbstractFile* packFile) = 0;

	//
	// 清空文件夹下的所有文件
	//
	virtual bool clear() = 0;

	//
	// 查找空闲的文件槽位
	//
	virtual int findAvailableSlot() = 0;

protected:

	//
	// 目录下的文件是否已经加载到内存
	//
	virtual bool isFileLoaded(unsigned int index) = 0;

	//
	// 获取加载到内存中的文件
	//
	virtual YCIAbstractFile* getLoaded(unsigned int index) = 0;

protected:

	//
	// 上级目录
	//
	YCIFileDirectory* myParent;

	//
	// 所属Pack文件
	//
	YCIFilePack* myPack;
};

#endif


