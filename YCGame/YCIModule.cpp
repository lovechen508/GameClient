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
// ������name()
//
// Ŀ�ģ�ȡ��ģ������
//
const char* YCIModule::name()
{
	return myName;
}

//
// ������version()
//
// Ŀ�ģ�ȡ��ģ��汾
//
int YCIModule::version()
{
	return myVersion;
}

//
// ������status()
//
// Ŀ�ģ�ȡ��ģ��״̬
//
int YCIModule::status()
{
	return myStatus;
}

//
// ������depend()
//
// Ŀ�ģ�����ģ�������б�
//
YCDList* YCIModule::dependencies()
{
	return myDependencies;
}