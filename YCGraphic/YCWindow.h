#ifndef _INCLUDE_YCWINDOW_H_
#define _INCLUDE_YCWINDOW_H_

#include "YCUITagDef.h"
#include "YCIContainer.h"

class YCWindow :
	public YCIContainer
{
public:

	YCWindow(const YCUITagMeta* tagMeta);

	virtual ~YCWindow(void);
	
	//
	// 函数：setGroup(const char* group)
	//
	// 目的：设置窗口组别
	//
	void setGroup(const char* group);

	//
	// 函数：getGroup()
	//
	// 目的：获取窗口组别
	//
	const char* getGroup();

	//
	// 函数：setWeight(int weight)
	//
	// 目的：设置窗口权重
	//
	void setWeight(int weight);

	//
	// 函数：getWeight()
	//
	// 目的：获取窗口权重
	//
	int getWeight();

	//
	// 函数：matchAccelerateKey()
	//
	// 目的：是否匹配快捷键
	//
	virtual bool matchAccelerateKey();

	//
	// 函数：clone()
	//
	// 目的：克隆当前类
	//
	virtual YCWindow* clone();
	
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
	// 函数：initAccelerateKey()
	//
	// 目的：处理快捷键
	//
	virtual void initAccelerateKey();

private:

	// 目前只支持两个键合成一个快捷键
	BYTE myAccelerateKey[MAX_ACCELERATE_KEYS];

	int myWeight;

	char myGroup[GROUP_NAME_LENGTH];
};

#endif