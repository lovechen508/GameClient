#ifndef _INCLUDE_YCDIALOG_H_
#define _INCLUDE_YCDIALOG_H_

#include "YCIContainer.h"

class YCDialog :
	public YCIContainer
{
public:

	YCDialog(const YCUITagMeta* tagMeta);

	virtual ~YCDialog(void);

private:

	//
	// 函数：preload()
	//
	// 目的：准备数据等，每次打开窗口前调用
	//
	virtual bool preload();
	
	//
	// 函数：postunload()
	//
	// 目的：准备数据等，每次关闭窗口前调用
	//
	virtual bool postunload();

	//
	// 函数：clone()
	//
	// 目的：克隆当前类
	//
	virtual YCDialog* clone();
};

#endif
