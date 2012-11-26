#ifndef _INCLUDE_YCLYCFILEPACK_H_
#define _INCLUDE_YCLYCFILEPACK_H_

#include "YCBasic\YCIFilePack.h"
#include "YCLYCDefine.h"

class YCDList;

//
// YCLYCFilePack : LYC 包文件
//
// 说明：
//       myLength = 文件总长度 - sizeof(LYC_PACK_T_1)
//                  在文件尾追加需要增加这个值
//
class YCAPI YCLYCFilePack :
	public YCIFilePack
{
public:
	
	YCLYCFilePack(const char* name, FILE* fd, LYC_PACK_T_1* header, YCIPackFileBuilder* builder);
	
	virtual ~YCLYCFilePack(void);

	//
	// 取得包名
	//
	virtual const char* getPackName();

	//
	// 注释：取得文件头
	//
	LYC_PACK_T_1* getHeader();

	//
	// 注释：返回文件头Header长度
	//
	virtual int getLength();

	//
	// 注释：返回文件内容指针
	//
	virtual const char* getContent();

	//
	// 注释：设置文件除头部内容区长度
	//
	virtual void setContentLength(int contentLen);

	//
	// 注释：获取文件除头部内容区长度
	//
	virtual int getContentLength();

	//
	// 注释：添加磁盘reguleFile到Pack包中
	//
	virtual bool add(const char* reguleFile, bool overwrite);

	//
	// 注释：添加内存文件到Pack包中
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool overwrite);

	//
	// 注释：添加子目录，是否递归
	//
	virtual bool mkdir(const char* dir, bool recursive);

	//
	// 注释：是否包含fileInPack
	//
	virtual int has(const char* fileInPack);

	//
	// 注释：抽取packFile文件
	//
	virtual YCIAbstractFile* get(const char* packFile);

	//
	// 注释：删除packFile文件，找到置标志位即可
	//
	virtual bool remove(const char* packFile);

	//
	// 注释：删除packFile文件，找到置标志位即可
	//
	virtual bool remove(YCIAbstractFile* packFile);

	//
	// 注释：清空文件夹下的所有文件
	//
	virtual bool clear();

	//
	// 注释：导出Pack包中所有文件
	//
	virtual void dump(const char* targetPath, bool overwrite);
	
	//
	// 注释：在PackFile有了改动之后，强制刷新到磁盘
	//
	virtual void flush();

	//
	// 注释：查找空闲的文件槽位
	//
	virtual int findAvailableSlot();
	
	//
	// 挂接空闲块
	//
	virtual void addIdleBlock(unsigned int offset, unsigned int length);

	//
	// 查找空闲的文件块
	//
	virtual IDLE_BLOCK* findAvailableBlock(unsigned int required);

protected:

	//
	// 注释：目录下的文件是否已经加载到内存
	//
	virtual bool isFileLoaded(unsigned int index);

	//
	// 注释：获取加载到内存中的文件
	//
	virtual YCIAbstractFile* getLoaded(unsigned int index);

	//
	// 注释：获取子文件在内存中的镜像
	//
	YCIAbstractFile* getFile(unsigned int index, const LYC_FILE_T_1& file);

	//
	// 注释：加载文件到内存，构建YCIAbstractFile到myFiles
	//
	YCIAbstractFile* loadFile(unsigned int index, const LYC_FILE_T_1& file);

private:

	//
	// 包名，去掉文件路径和扩展名
	//
	char myPackName[FILENAME_LENGTH];
	
	//
	// 文件块，持有生命周期
	//
	LYC_PACK_T_1* myHeader;

	//
	// 已经加载的文件
	//
	YCIAbstractFile* myFiles[FILES_IN_DIR];
};

#endif

