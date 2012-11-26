#ifndef _INCLUDE_YCIMODULELOADER_H_
#define _INCLUDE_YCIMODULELOADER_H_

class YCConfig;

/*
 * YCIModuleLoader : 模块网络下载接口类
 *
 */
class YCAPI YCIModuleLoader
{
public:

	YCIModuleLoader();

	virtual ~YCIModuleLoader();

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：配置模块下载器
	//
	virtual bool initialize(YCConfig& config) = 0;

	//
	// 函数：download(const char* name, int version)
	//
	// 目的：下载对应名称和版本的模块
	//       下载成功返回true，否则返回false
	//
	virtual bool download(const char* name, int version) = 0;
};

#endif