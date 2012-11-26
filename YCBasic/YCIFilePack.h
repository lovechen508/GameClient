#ifndef _INCLUDE_YCIPACKFILE_H_
#define _INCLUDE_YCIPACKFILE_H_

#include <stdio.h>
#include "YCDef.h"

#include "YCIFileDirectory.h"

class YCDList;
class YCIFileNormal;
class YCIPackFileBuilder;

/*
 * YCIPackFile : 打包文件抽象类：如*.mpq
 *               继承自YCIFileHandle是便于实现自身递归包含
 *               支持增，查，删
 */
class YCAPI YCIFilePack : public YCIFileDirectory
{
public:

	//
	// 说明：空闲块结构
	//
	struct IDLE_BLOCK {

		IDLE_BLOCK() : offset(0), length(0) {}

		IDLE_BLOCK(unsigned int o, unsigned int l) : offset(o), length(l) {}

		unsigned int offset;               //当前空闲块的偏移量
		unsigned int length;			   //当前空闲块的大小，必须为FILE_ROUNDSIZE的整数倍
	};

public:

	YCIFilePack(const char* name, FILE* fd, YCIPackFileBuilder* builder);

	virtual ~YCIFilePack(void);

	//
	// 取得包名
	//
	virtual const char* getPackName() = 0;

	//
	// 取得文件在包文件的偏移量
	//
	virtual unsigned int getOffset();

	//
	// 注释：设置文件除头部内容区长度
	//
	virtual void setContentLength(int contentLen) = 0;

	//
	// 注释：获取文件除头部内容区长度
	//
	virtual int getContentLength() = 0;

	//
	// 获取普通文件读写助手类
	//
	YCIPackFileBuilder* getBuilder();
	
	//
	// 挂接空闲块
	//
	virtual void addIdleBlock(unsigned int offset, unsigned int length) = 0;

	//
	// 查找空闲的文件块
	//
	virtual IDLE_BLOCK* findAvailableBlock(unsigned int required) = 0;

protected:

	//
	// 文件加载，写入帮助类
	//
	YCIPackFileBuilder * myBuilder;
	
	//
	// 空闲块链表
	//
	YCDList * myIdleList;
};

#endif

