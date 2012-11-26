#ifndef _INCLUDE_YCIFILENORMAL_H_
#define _INCLUDE_YCIFILENORMAL_H_

#include "YCDef.h"
#include "YCIAbstractFile.h"

class YCIFilePack;
class YCIFileDirectory;

/*
 * YCIFileNormal : YCIPackFile中一个文件的抽象
 */
class YCAPI YCIFileNormal : public YCIAbstractFile
{
public:

	YCIFileNormal(YCIFilePack* pkg, const char* name);

	virtual ~YCIFileNormal(void);

	//
	// 设置父目录，有可能是YCIPackFile
	//
	void setParent(YCIFileDirectory* parent);

	//
	// 获取父目录
	//
	YCIFileDirectory* getParent() const;

	//
	// 注释：设置文件长度
	//
	void setLength(int len);

	//
	// 注释：返回文件长度
	//
	virtual int getLength();

	//
	// 取得文件所在包
	//
	YCIFilePack* getPack();

	//
	// 设置数据，从外部文件创建时, 
	// 长度在初始化时固定
	// 接管生命周期
	//
	void setContent(char* buf);

protected:

	//
	// 所属Pack文件
	//
	YCIFilePack* myPack;

	//
	// 所属文件夹
	//
	YCIFileDirectory * myParent;

	//
	// 文件内容, 已解密
	//
	char* myContent;

	//
	// 属性：文件自身长度, 已解密
	//
	int myLength;
};

#endif

