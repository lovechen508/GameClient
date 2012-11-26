#ifndef _INCLUDE_YCMODULEMANAGER_H_
#define _INCLUDE_YCMODULEMANAGER_H_

#include "YCBasic\YCIDumper.h"

class YCDList;
class YCIModule;
class YCIModuleLoader;

//
// YCModuleManager : ģ�����������������
//                   ���ģ��ļ��أ�ж��
// 
// ע�ͣ����ģ���ѭ������
//
class YCModuleManager : public YCIDumper
{
public:

	YCModuleManager();

	virtual ~YCModuleManager();

	//
	// ������setModuleLoader(YCIModuleLoader* loader)
	//
	// Ŀ�ģ�����ģ�������,�ظ����û��׳�YCException
	//
	void setModuleLoader(YCIModuleLoader* loader);

	//
	// ������initialize()
	//
	// Ŀ�ģ�����Ŀ¼�µ�xml module�����ļ�
	//       ���������Դ������module�б�
	//
	bool initialize(const char* path);

	//
	// ������find(const char* moduleName, int minVersion)
	//
	// Ŀ�ģ����ģ����ΪmoduleName����СminVersion��Module�Ƿ񱾵ش���
	//       ��������ͬģ������version����һ��ģ�飬
	//       �Ա�֤ʼ�ռ������µ�module
	//
	YCIModule* find(const char* moduleName, int minVersion);

	//
	// ������exist(const char* moduleName, int minVersion)
	//
	// Ŀ�ģ����ģ����ΪmoduleName����СminVersion��Module�Ƿ񱾵ش���
	//
	bool exist(const char* moduleName, int minVersion);

	//
	// ������download(const char* moduleName, int version)
	//
	// Ŀ�ģ�����YCIModuleLoader��������Module
	//
	bool download(const char* moduleName, int version);

	//
	// ������load(YCIModule* module)
	//
	// Ŀ�ģ����ض�Ӧģ��
	//
	void load(YCIModule* module);

	//
	// ������unload(YCIModule* module)
	//
	// Ŀ�ģ�ж�ض�Ӧģ��
	//
	void unload(YCIModule* module);

	//
	// ������reload(YCIModule* module)
	//
	// Ŀ�ģ����ض�Ӧģ��
	//
	void reload(YCIModule* module);

	//
	// ������dump()
	//
	// Ŀ�ģ�dump���ڲ�״̬ΪHtmlƬ��
	//
	// ע�ͣ��̳���YCIDumper�ӿ���
	//
	virtual const char* dump();

private:

	//
	// ������compile(const char* moduleName, int version)
	//
	// Ŀ�ģ����벢����ָ������ģ��
	//      ����ȷ�������Ѵ��ڣ��������YCException
	//
	YCIModule* compile(const char* moduleName, int version);

private:

	YCIModuleLoader* myModuleLoader;

	YCDList* myModules;
};

#endif