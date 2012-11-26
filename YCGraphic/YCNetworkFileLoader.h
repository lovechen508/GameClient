#ifndef _INCLUDE_YCNETWORKFILELOADER_H_
#define _INCLUDE_YCNETWORKFILELOADER_H_

#include <map>

#include "YCBasic\YCLock.h"
#include "YCBasic\YCThreadPool.h"

class YCConfig;
class YCNetworkFileLoadListener;

/*
 * YCNetworkFileLoader : ����Զ���ļ����������
 */
class YCNetworkFileLoader 
{
	
	enum E_DOWNLOAD_STATUS {DOWNLOADING = 0, SUCCESSED = 1, FAILED = 2};

public:

	YCNetworkFileLoader();
	
	virtual ~YCNetworkFileLoader(void);
		
	//
	// ������initialize()
	//
	// Ŀ�ģ����������������
	//
	bool initialize(YCConfig& config);

	//
	// ������request(YCNetworkFileLoaderListener* listener, const char* filename)
	//
	// Ŀ�ģ���������ʵ�ֺ���
	//
	// ������filenameΪ����URL
	//
	virtual void request(YCNetworkFileLoadListener* listener, const char* filename);

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷ�Http�������ӳ�   
	//       �ͷ��ڲ����ݽṹ
	//
	void finalize();

private:

	//
	// ������addToDownloadMap(const char* filename)
	//
	// Ŀ�ģ���ӵ������б���,��ֹ�ظ�����
	//
	bool addToDownloadMap(const char* filename);

private:

	int myNoOfConnections;

	std::string myDownloadServerURL;
	
	//
	// ���ԣ�myDownloadMap
	//
	// Key�������ļ���
	// Value������״̬ 0, �������� 1, ���سɹ�
	//        �������ʧ�ܣ����map���Ƴ�����YCTextureManager�е�ʧЧ��������������
	std::map<std::string, int> myDownloadMap;

	YCLock myMapLock;
		
	YCThreadPool* myHttpClients;
};

#endif

