#ifndef _INCLUDE_YCHTTPMODULELOADER_H_
#define _INCLUDE_YCHTTPMODULELOADER_H_

#include "YCIModuleLoader.h"

class YCHttpModuleLoader : public YCIModuleLoader
{
public:

	YCHttpModuleLoader();

	virtual ~YCHttpModuleLoader();

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ�����ģ��������
	//
	virtual bool initialize(YCConfig& config);

	//
	// ������download(const char* name, int version)
	//
	// Ŀ�ģ����ض�Ӧ���ƺͰ汾��ģ��
	//       ���سɹ�����true�����򷵻�false
	//
	virtual bool download(const char* name, int version);

private:

	const char* myURL;
};

#endif