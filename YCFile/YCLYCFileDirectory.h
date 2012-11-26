#ifndef _INCLUDE_YCLYCFILEDIRECTORY_H_
#define _INCLUDE_YCLYCFILEDIRECTORY_H_

#include "YCLYCDefine.h"
#include "YCBasic\YCIFileDirectory.h"

class YCAPI YCLYCFileDirectory :
	public YCIFileDirectory
{
public:
	
	YCLYCFileDirectory(YCIFilePack* pkg, const char* filename);
	
	virtual ~YCLYCFileDirectory(void);

	//
	// 设置自身Header
	//
	void setSelfHeader(LYC_FILE_T_1* header);

	//
	// 设置文件头
	//
	void setHeader(LYC_DIR_T_1* header);

	//
	// 注释：取得文件头
	//
	LYC_DIR_T_1* getHeader();

	//
	// 注释：返回文件内容指针
	//
	virtual const char* getContent();

	//
	// 取得文件在包文件的偏移量
	//
	virtual unsigned int getOffset();

	//
	// 注释：返回文件长度
	//
	virtual int getLength();

	//
	// 添加磁盘reguleFile到目录中
	//
	virtual bool add(const char* reguleFile, bool overwrite);

	//
	// 添加内存文件到目录中
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool overwrite);

	//
	// 添加子目录，是否递归
	//
	virtual bool mkdir(const char* dir, bool recursive);

	//
	// 是否包含fileInPack, 存在返回文件类型，否则返回0
	//
	virtual int has(const char* fileInPack);

	//
	// 抽取packFile文件
	//
	virtual YCIAbstractFile* get(const char* packFile);

	//
	// 删除packFile文件，找到置标志位即可
	//
	virtual bool remove(const char* packFile);

	//
	// 删除packFile文件，找到置标志位即可
	//
	virtual bool remove(YCIAbstractFile* packFile);

	//
	// 清空文件夹下的所有文件
	//
	virtual bool clear();

	//
	// 导出目录中所有文件
	//
	virtual void dump(const char* targetPath, bool overwrite);

	//
	// 在有了改动之后，强制刷新到磁盘
	//
	virtual void flush();

	//
	// 查找空闲的文件槽位
	//
	virtual int findAvailableSlot();
	
protected:
	
	//
	// 目录下的文件是否已经加载到内存
	//
	virtual bool isFileLoaded(unsigned int index);

	//
	// 获取加载到内存中的文件
	//
	virtual YCIAbstractFile* getLoaded(unsigned int index);

	//
	// 获取子文件在内存中的镜像
	//
	YCIAbstractFile* getFile(unsigned int index, const LYC_FILE_T_1& file);

	//
	// 加载文件到内存，构建YCIAbstractFile到myFiles
	//
	YCIAbstractFile* loadFile(unsigned int index, const LYC_FILE_T_1& file);

private:

	//
	// 文件头描述，存储引用
	//
	const LYC_FILE_T_1* mySelfHeader;

	//
	// 目录中的下级文件，持有生命周期
	//
	LYC_DIR_T_1* myHeader;

	//
	// 已经加载的文件
	//
	YCIAbstractFile* myFiles[FILES_IN_DIR];

};

#endif