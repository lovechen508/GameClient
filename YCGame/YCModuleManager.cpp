#include "YCModuleManager.h"

#include "YCIModule.h"
#include "YCIModuleLoader.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"


YCModuleManager::YCModuleManager()
	: myModuleLoader(NULL)
	, myModules(new YCDList())
{
}

YCModuleManager::~YCModuleManager()
{
	SAFE_DELETE(myModuleLoader);
	SAFE_DELETE(myModules);
}

//
// 函数：setModuleLoader(YCIModuleLoader* loader)
//
// 目的：设置模块加载器
//
void YCModuleManager::setModuleLoader(YCIModuleLoader* loader)
{
	SMART_ASSERT(myModuleLoader == NULL);
	if (myModuleLoader != NULL)
	{
		throw YCException(2002, "YCModuleManager::setModuleLoader重复设置!");
	}
	myModuleLoader = loader;
}

//
// 函数：initialize()
//
// 目的：遍历目录下的xml module配置文件
//       加载相关资源，构建module列表
//
bool YCModuleManager::initialize(const char* path)
{
	SMART_ASSERT(path != NULL);
	return true;
}

//
// 函数：find(const char* moduleName, int minVersion)
//
// 目的：检查模块名为moduleName的最小minVersion的Module是否本地存在
//
YCIModule* YCModuleManager::find(const char* moduleName, int minVersion)
{
	SMART_ASSERT(moduleName != NULL);
	SMART_ASSERT(minVersion > 0);

	YCIModule *result = NULL;

	for (Item_List_T* it = myModules->begin(); 
		 it != myModules->end();
		 it = myModules->next(it))
	{
		YCIModule* module = (YCIModule*)(myModules->payload(it));
		if (strcmp(module->name(), moduleName) == 0)
		{
			if (result == NULL)
			{
				result = module;
			}
			else if (module->version() > result->version())
			{
				result = module;
			}
		}
	}
	
	return result;
}

//
// 函数：exist(const char* moduleName, int minVersion)
//
// 目的：检查模块名为moduleName的最小minVersion的Module是否本地存在
//
bool YCModuleManager::exist(const char* moduleName, int minVersion)
{
	return (NULL != find(moduleName, minVersion));
}

//
// 函数：download(const char* moduleName, int version)
//
// 目的：调用YCIModuleLoader阻塞下载Module
//
bool YCModuleManager::download(const char* moduleName, int version)
{
	SMART_ASSERT(myModuleLoader != NULL);
	if (!exist(moduleName, version))
	{
		if (!myModuleLoader->download(moduleName, version))
		{
			return false;
		}

		YCIModule * module = compile(moduleName, version);
		if (module == NULL)
		{
			return false;
		}

		myModules->append(module, NULL); 
	}
	return true;
}

//
// 函数：load(YCIModule* module)
//
// 目的：加载对应模块
//
void load(YCIModule* module)
{
	SMART_ASSERT(module != NULL);
	if (module == NULL)
	{
		throw YCException(2002, "YCModuleManager::load模块为空!");
	}

	if (module->status() == YCIModule::INITED)
	{
		//module->preload();

		module->load();

		//module->postload();
	}
}

//
// 函数：unload(YCIModule* module)
//
// 目的：卸载对应模块
//
void YCModuleManager::unload(YCIModule* module)
{
	SMART_ASSERT(module != NULL);

	if (module == NULL)
	{
		throw YCException(2002, "YCModuleManager::unload模块为空!");
	}

	if (module->status() == YCIModule::ACTIVE)
	{
		//module->preunload();

		module->unload();

		//module->postunload();
	}
}

//
// 函数：reload(YCIModule* module)
//
// 目的：重载对应模块
//
void YCModuleManager::reload(YCIModule* module)
{
	SMART_ASSERT(module != NULL);

	if (module == NULL)
	{
		throw YCException(2002, "YCModuleManager::reload模块为空!");
	}

	if (module->status() == YCIModule::ACTIVE)
	{
		//module->preunload();
		module->unload();
		//module->postunload();

		//module->preload();
		module->load();
		//module->postload();
	}
}

//
// 函数：dump()
//
// 目的：dump类内部状态为Html片段
//
// 注释：继承自YCIDumper接口类
//
const char* YCModuleManager::dump()
{
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// Private Part
///////////////////////////////////////////////////////////////////////////////////

//
// 函数：compile(const char* moduleName, int version)
//
// 目的：编译并加载指定本地模块
//      必须确保本地已存在，否则会抛YCException
//
YCIModule* YCModuleManager::compile(const char* moduleName, int version)
{
	return NULL;
}