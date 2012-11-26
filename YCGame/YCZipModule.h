#ifndef _INCLUDE_YCZIPMODULE_H_
#define _INCLUDE_YCZIPMODULE_H_

#include "YCIModule.h"

class YCZipModule : public YCIModule
{
public:

	YCZipModule();

	virtual ~YCZipModule();

	//
	// ������initialize()
	//
	// Ŀ�ģ�������ɺ�ĳ�ʼ����ģ��״̬ת��ΪINITED
	//
	virtual bool initialize();

	//
	// ������preload()
	//
	// Ŀ�ģ�����ǰ��׼������
	//
	virtual void preload();

	//
	// ������load()
	//
	// Ŀ�ģ�ִ��������ģ�����
	//       ע�᳡���㣬ע�ᴰ���࣬������Դ
	//
	virtual void load();

	//
	// ������postload����
	//
	// Ŀ�ģ�������ɺ�ĸ�������
	//
	virtual void postload();

	//
	// ������preunload()
	//
	// Ŀ�ģ�ж��ǰ��׼������
	//
	virtual void preunload();

	//
	// ������load()
	//
	// Ŀ�ģ�ִ��������ģ��ж��
	//       ж�س����㣬ж�ش����࣬ж����Դ
	//
	virtual void unload();

	//
	// ������postunload����
	//
	// Ŀ�ģ�ж����ɺ�ĸ�������
	//
	virtual void postunload();
};

#endif