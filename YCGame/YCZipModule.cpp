#include "YCZipModule.h"

YCZipModule::YCZipModule()
	: YCIModule()
{
}

YCZipModule::~YCZipModule()
{
}

//
// ������initialize()
//
// Ŀ�ģ�������ɺ�ĳ�ʼ����ģ��״̬ת��ΪINITED
//
bool YCZipModule::initialize()
{
	myStatus = INITED;
	return true;
}

//
// ������preload()
//
// Ŀ�ģ�����ǰ��׼������
//
void YCZipModule::preload()
{
}

//
// ������load()
//
// Ŀ�ģ�ִ��������ģ�����
//       ע�᳡���㣬ע�ᴰ���࣬������Դ
//
void YCZipModule::load()
{
	myStatus = ACTIVE;
}

//
// ������postload����
//
// Ŀ�ģ�������ɺ�ĸ�������
//
void YCZipModule::postload()
{
}

//
// ������preunload()
//
// Ŀ�ģ�ж��ǰ��׼������
//
void YCZipModule::preunload()
{
}

//
// ������load()
//
// Ŀ�ģ�ִ��������ģ��ж��
//       ж�س����㣬ж�ش����࣬ж����Դ
//
void YCZipModule::unload()
{
	myStatus = INITED;
}

//
// ������postunload����
//
// Ŀ�ģ�ж����ɺ�ĸ�������
//
void YCZipModule::postunload()
{
}