#ifndef _INCLUDE_YCHTTPMODULELOADER_H_
#define _INCLUDE_YCHTTPMODULELOADER_H_

#include "YCIModuleLoader.h"

class YCHttpModuleLoader : public YCIModuleLoader
{
public:

	YCHttpModuleLoader();

	virtual ~YCHttpModuleLoader();

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：配置模块下载器
	//
	virtual bool initialize(YCConfig& config);

	//
	// 函数：download(const char* name, int version)
	//
	// 目的：下载对应名称和版本的模块
	//       下载成功返回true，否则返回false
	//
	virtual bool download(const char* name, int version);

private:

	const char* myURL;
};

#endif