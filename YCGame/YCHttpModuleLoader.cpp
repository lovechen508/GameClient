#include "YCHttpModuleLoader.h"

YCHttpModuleLoader::YCHttpModuleLoader()
{
}

YCHttpModuleLoader::~YCHttpModuleLoader()
{
}

//
// ������initialize(YCConfig& config)
//
// Ŀ�ģ�����ģ��������
//
bool YCHttpModuleLoader::initialize(YCConfig& config)
{
	return true;
}

//
// ������download(const char* name, int version)
//
// Ŀ�ģ����ض�Ӧ���ƺͰ汾��ģ��
//       ���سɹ�����true�����򷵻�false
//
bool YCHttpModuleLoader::download(const char* name, int version)
{
	return false;
}