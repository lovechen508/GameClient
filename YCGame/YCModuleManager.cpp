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
// ������setModuleLoader(YCIModuleLoader* loader)
//
// Ŀ�ģ�����ģ�������
//
void YCModuleManager::setModuleLoader(YCIModuleLoader* loader)
{
	SMART_ASSERT(myModuleLoader == NULL);
	if (myModuleLoader != NULL)
	{
		throw YCException(2002, "YCModuleManager::setModuleLoader�ظ�����!");
	}
	myModuleLoader = loader;
}

//
// ������initialize()
//
// Ŀ�ģ�����Ŀ¼�µ�xml module�����ļ�
//       ���������Դ������module�б�
//
bool YCModuleManager::initialize(const char* path)
{
	SMART_ASSERT(path != NULL);
	return true;
}

//
// ������find(const char* moduleName, int minVersion)
//
// Ŀ�ģ����ģ����ΪmoduleName����СminVersion��Module�Ƿ񱾵ش���
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
// ������exist(const char* moduleName, int minVersion)
//
// Ŀ�ģ����ģ����ΪmoduleName����СminVersion��Module�Ƿ񱾵ش���
//
bool YCModuleManager::exist(const char* moduleName, int minVersion)
{
	return (NULL != find(moduleName, minVersion));
}

//
// ������download(const char* moduleName, int version)
//
// Ŀ�ģ�����YCIModuleLoader��������Module
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
// ������load(YCIModule* module)
//
// Ŀ�ģ����ض�Ӧģ��
//
void load(YCIModule* module)
{
	SMART_ASSERT(module != NULL);
	if (module == NULL)
	{
		throw YCException(2002, "YCModuleManager::loadģ��Ϊ��!");
	}

	if (module->status() == YCIModule::INITED)
	{
		//module->preload();

		module->load();

		//module->postload();
	}
}

//
// ������unload(YCIModule* module)
//
// Ŀ�ģ�ж�ض�Ӧģ��
//
void YCModuleManager::unload(YCIModule* module)
{
	SMART_ASSERT(module != NULL);

	if (module == NULL)
	{
		throw YCException(2002, "YCModuleManager::unloadģ��Ϊ��!");
	}

	if (module->status() == YCIModule::ACTIVE)
	{
		//module->preunload();

		module->unload();

		//module->postunload();
	}
}

//
// ������reload(YCIModule* module)
//
// Ŀ�ģ����ض�Ӧģ��
//
void YCModuleManager::reload(YCIModule* module)
{
	SMART_ASSERT(module != NULL);

	if (module == NULL)
	{
		throw YCException(2002, "YCModuleManager::reloadģ��Ϊ��!");
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
// ������dump()
//
// Ŀ�ģ�dump���ڲ�״̬ΪHtmlƬ��
//
// ע�ͣ��̳���YCIDumper�ӿ���
//
const char* YCModuleManager::dump()
{
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// Private Part
///////////////////////////////////////////////////////////////////////////////////

//
// ������compile(const char* moduleName, int version)
//
// Ŀ�ģ����벢����ָ������ģ��
//      ����ȷ�������Ѵ��ڣ��������YCException
//
YCIModule* YCModuleManager::compile(const char* moduleName, int version)
{
	return NULL;
}