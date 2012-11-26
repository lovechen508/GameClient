#ifndef _INCLUDE_YCIMODULELOADER_H_
#define _INCLUDE_YCIMODULELOADER_H_

class YCConfig;

/*
 * YCIModuleLoader : ģ���������ؽӿ���
 *
 */
class YCAPI YCIModuleLoader
{
public:

	YCIModuleLoader();

	virtual ~YCIModuleLoader();

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ�����ģ��������
	//
	virtual bool initialize(YCConfig& config) = 0;

	//
	// ������download(const char* name, int version)
	//
	// Ŀ�ģ����ض�Ӧ���ƺͰ汾��ģ��
	//       ���سɹ�����true�����򷵻�false
	//
	virtual bool download(const char* name, int version) = 0;
};

#endif