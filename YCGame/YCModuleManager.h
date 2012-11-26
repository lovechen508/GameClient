#ifndef _INCLUDE_YCMODULEMANAGER_H_
#define _INCLUDE_YCMODULEMANAGER_H_

#include "YCBasic\YCIDumper.h"

class YCDList;
class YCIModule;
class YCIModuleLoader;

//
// YCModuleManager : 模块管理器，管理依赖
//                   完成模块的加载，卸载
// 
// 注释：检查模块的循环依赖
//
class YCModuleManager : public YCIDumper
{
public:

	YCModuleManager();

	virtual ~YCModuleManager();

	//
	// 函数：setModuleLoader(YCIModuleLoader* loader)
	//
	// 目的：设置模块加载器,重复设置会抛出YCException
	//
	void setModuleLoader(YCIModuleLoader* loader);

	//
	// 函数：initialize()
	//
	// 目的：遍历目录下的xml module配置文件
	//       加载相关资源，构建module列表
	//
	bool initialize(const char* path);

	//
	// 函数：find(const char* moduleName, int minVersion)
	//
	// 目的：检查模块名为moduleName的最小minVersion的Module是否本地存在
	//       将返回相同模块名，version最大的一个模块，
	//       以保证始终加载最新的module
	//
	YCIModule* find(const char* moduleName, int minVersion);

	//
	// 函数：exist(const char* moduleName, int minVersion)
	//
	// 目的：检查模块名为moduleName的最小minVersion的Module是否本地存在
	//
	bool exist(const char* moduleName, int minVersion);

	//
	// 函数：download(const char* moduleName, int version)
	//
	// 目的：调用YCIModuleLoader阻塞下载Module
	//
	bool download(const char* moduleName, int version);

	//
	// 函数：load(YCIModule* module)
	//
	// 目的：加载对应模块
	//
	void load(YCIModule* module);

	//
	// 函数：unload(YCIModule* module)
	//
	// 目的：卸载对应模块
	//
	void unload(YCIModule* module);

	//
	// 函数：reload(YCIModule* module)
	//
	// 目的：重载对应模块
	//
	void reload(YCIModule* module);

	//
	// 函数：dump()
	//
	// 目的：dump类内部状态为Html片段
	//
	// 注释：继承自YCIDumper接口类
	//
	virtual const char* dump();

private:

	//
	// 函数：compile(const char* moduleName, int version)
	//
	// 目的：编译并加载指定本地模块
	//      必须确保本地已存在，否则会抛YCException
	//
	YCIModule* compile(const char* moduleName, int version);

private:

	YCIModuleLoader* myModuleLoader;

	YCDList* myModules;
};

#endif