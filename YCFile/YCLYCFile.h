#ifndef _INCLUDE_YCLYCFILE_H_
#define _INCLUDE_YCLYCFILE_H_

#include "YCBasic\YCIFileNormal.h"

struct LYC_FILE_T_1;

class YCAPI YCLYCFile :
	public YCIFileNormal
{
public:
	
	YCLYCFile(YCIFilePack* pkg, const char* filename);
	
	virtual ~YCLYCFile(void);

	//
	// 设置自身Header
	//
	void setSelfHeader(LYC_FILE_T_1* header);

	//
	// 在有了改动之后，强制刷新到磁盘
	//
	virtual void flush();

	//
	// 注释：返回文件内容指针
	//
	virtual const char* getContent();

	//
	// 取得文件在包文件的偏移量
	//
	virtual unsigned int getOffset();
	
	//
	// 导出自身文件
	//
	virtual void dump(const char* targetPath, bool overwrite);

private:

	//
	// 文件头描述，存储引用
	//
	LYC_FILE_T_1* mySelfHeader;
};

#endif