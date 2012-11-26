#include "YCHttpModuleLoader.h"

YCHttpModuleLoader::YCHttpModuleLoader()
{
}

YCHttpModuleLoader::~YCHttpModuleLoader()
{
}

//
// 函数：initialize(YCConfig& config)
//
// 目的：配置模块下载器
//
bool YCHttpModuleLoader::initialize(YCConfig& config)
{
	return true;
}

//
// 函数：download(const char* name, int version)
//
// 目的：下载对应名称和版本的模块
//       下载成功返回true，否则返回false
//
bool YCHttpModuleLoader::download(const char* name, int version)
{
	return false;
}