#ifndef _INCLUDE_YCIABSTRACTFILE_H_
#define _INCLUDE_YCIABSTRACTFILE_H_

#include <stdlib.h>

#include "YCAssert.h"
#include "YCException.h"

/*
 * YCIAbstractFile : 打包文件抽象接口类
 *
 *                                   -------------------
 *                                  |  YCIAbstractFile  |
 *                                   -------------------
 *                                            |
 *                         --------------------------------------         
 *                        |                                      |
 *               -----------------                       ------------------ 
 *              |  YCIFileNormal  |                     | YCIFileDirectory |
 *               -----------------                       ------------------
 *                                                               |
 *                                                       ------------------                                                   
 *                                                      |   YCIFilePack    |
 *                                                       ------------------
 *
 */
class YCAPI YCIAbstractFile
{
public:

	enum {
		FILE_NORMAL  = 1, //正常文件
		FILE_DIR     = 2, //目录文件
		FILE_PACK	 = 3, //包文件
	};

public:

	YCIAbstractFile(const char* name, int type)
		: myType(type), myFd(NULL)
	{
		SMART_ASSERT(name != NULL);
		if (strlen(name) >= MAX_PATH)
		{
			throw YCException(1002, "YCIFileHandle文件名过长");
		}
		memcpy(myName, name, strlen(name));
		myName[strlen(name)] = '\0';
	}

	virtual ~YCIAbstractFile() {}

	//
	// 注释：返回文件对应数据
	//
	//    Normal : 文件在内存中的长度，（plain）
	//    Dir    : 头部长度
	//    Pack   : 包头长度
	//
	virtual const char* getContent() = 0;
	
	//
	// 在有了改动之后，强制刷新到磁盘
	//
	virtual void flush() = 0;

	//
	// 取得文件在包文件的偏移量
	//
	virtual unsigned int getOffset() = 0;
	
	//
	// 导出文件
	//
	virtual void dump(const char* targetPath, bool overwrite) = 0;

	//
	// 已修改
	//
	void dirty()
	{
		myDirty = true;
	}

	//
	// 是否已修改
	//
	bool isDirty() const
	{
		return myDirty;
	}

	//
	// 绑定到文件指针
	//
	void bind(FILE* fd)
	{
		myFd = fd;
	}

	//
	// 文件类型
	//
	int type() const
	{
		return myType; 
	}

	//
	// 取得文件描述符
	//
	FILE* getFd()
	{
		return myFd;
	}

	//
	// 文件名称
	//
	const char* filename() const
	{
		return myName;
	}

	//
	// 注释：返回文件长度
	//
	virtual int getLength() = 0;

protected:
	
	//
	// 文件指针
	//
	FILE * myFd;
	
	//
	// 属性：在打包文件中的文件名，全路径
	//
	char myName[MAX_PATH];

	//
	// 文件类型
	//
	const int myType;
	
	//
	// 属性：文件在内存中是否已修改
	//
	bool myDirty;
};

#endif