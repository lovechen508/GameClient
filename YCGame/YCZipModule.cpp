#include "YCZipModule.h"

YCZipModule::YCZipModule()
	: YCIModule()
{
}

YCZipModule::~YCZipModule()
{
}

//
// 函数：initialize()
//
// 目的：下载完成后的初始化，模块状态转换为INITED
//
bool YCZipModule::initialize()
{
	myStatus = INITED;
	return true;
}

//
// 函数：preload()
//
// 目的：加载前的准备工作
//
void YCZipModule::preload()
{
}

//
// 函数：load()
//
// 目的：执行真正的模块加载
//       注册场景层，注册窗口类，加载资源
//
void YCZipModule::load()
{
	myStatus = ACTIVE;
}

//
// 函数：postload（）
//
// 目的：加载完成后的附属动作
//
void YCZipModule::postload()
{
}

//
// 函数：preunload()
//
// 目的：卸载前的准备工作
//
void YCZipModule::preunload()
{
}

//
// 函数：load()
//
// 目的：执行真正的模块卸载
//       卸载场景层，卸载窗口类，卸载资源
//
void YCZipModule::unload()
{
	myStatus = INITED;
}

//
// 函数：postunload（）
//
// 目的：卸载完成后的附属动作
//
void YCZipModule::postunload()
{
}