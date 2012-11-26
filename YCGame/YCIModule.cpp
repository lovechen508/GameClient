#include "YCIModule.h"

#include "YCBasic\YCDList.h"

YCIModule::YCIModule()
	: myDependencies(new YCDList())
	, myStatus(LOADED)
{
}

YCIModule::~YCIModule()
{
	delete myDependencies;
}

//
// 函数：name()
//
// 目的：取得模块名称
//
const char* YCIModule::name()
{
	return myName;
}

//
// 函数：version()
//
// 目的：取得模块版本
//
int YCIModule::version()
{
	return myVersion;
}

//
// 函数：status()
//
// 目的：取得模块状态
//
int YCIModule::status()
{
	return myStatus;
}

//
// 函数：depend()
//
// 目的：返回模块依赖列表
//
YCDList* YCIModule::dependencies()
{
	return myDependencies;
}