#include "YCNetworkFileLoader.h"

#include "YCBasic\YCAssert.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLogger.h"
#include "YCConfig\YCConfig.h"
#include "YCBasic\YCLockGuard.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCThreadPool.h"
#include "YCNetwork\YCHttpClient.h"

#include "YCNetworkFileLoadListener.h"

#pragma region NetworkWorkItem

class NetworkWorkItem : public YCThreadPool::WorkItem
{
public:

	NetworkWorkItem(const char* baseUrl, const char* filename, YCNetworkFileLoadListener* listener)
	{
		SMART_ASSERT(baseUrl != NULL);
		SMART_ASSERT(filename != NULL);
		SMART_ASSERT(listener != NULL);
	}

	virtual ~NetworkWorkItem() {}

	//
	// ������void run();
	//
	// Ŀ�ģ����ÿ���������ʵ������
	//
	virtual void run() 
	{

	}

private:

	char myURL[MAX_PATH];
	char myFilename[MAX_PATH];

	YCNetworkFileLoadListener *myListener;
};

#pragma endregion NetworkWorkItem

YCNetworkFileLoader::YCNetworkFileLoader()
	: myNoOfConnections(0)
	, myHttpClients(NULL)
{

}


YCNetworkFileLoader::~YCNetworkFileLoader(void)
{
}

//
// ������configure()
//
// Ŀ�ģ����������������
//
bool YCNetworkFileLoader::initialize(YCConfig& config)
{
	SMART_ASSERT(myHttpClients == NULL);

	myDownloadServerURL = config.downloadServerURL();
	myNoOfConnections = config.noOfConnections();
	int maxItemsInQueue = config.maxItemsInQueue();
	
	if (myNoOfConnections == 0)
	{
		LOG_WARNING("�����������������Ϊ0���ر��������ع���!");
	}
	else
	{
		myNoOfConnections = min(max(myNoOfConnections, 1), 5);
		int realMaxItemsInQueue = min(max(maxItemsInQueue, 10), 100);
		LOG_INFO("������󲢷�������������" << myNoOfConnections);
		LOG_INFO("�������ȴ����У�" << realMaxItemsInQueue);
		myHttpClients = new YCThreadPool(myNoOfConnections, realMaxItemsInQueue);
	}

	return true;
}

//
// ������request(const char* filename)
//
// Ŀ�ģ���������ʵ�ֺ���
//
// ������filenameΪ����URL
//
void YCNetworkFileLoader::request(YCNetworkFileLoadListener* listener, const char* filename)
{
	if (listener == NULL)
	{
		throw YCException(2002, "YCNetworkFileLoader::createWorkItem��listenerΪNULL");
	}

	if (filename == NULL)
	{
		throw YCException(2002, "YCNetworkFileLoader::createWorkItem��filenameΪNULL");
	}

	if (myNoOfConnections == 0 || myHttpClients == NULL)
	{
		return; //������عر�
	}

	if (addToDownloadMap(filename))
	{
		myHttpClients->enqueue(new NetworkWorkItem(myDownloadServerURL.c_str(), filename, listener));
	}
}

//
// ������finalize()
//
// Ŀ�ģ��ͷ�Http�������ӳ�   
//       �ͷ��ڲ����ݽṹ
//
void YCNetworkFileLoader::finalize()
{
	if (myHttpClients != NULL)
	{
		myHttpClients->stop();
		delete myHttpClients;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////////////////////////////////////////

//
// ������addToDownloadMap(const char* filename)
//
// Ŀ�ģ���ӵ������б���,��ֹ�ظ�����
//
bool YCNetworkFileLoader::addToDownloadMap(const char* filename)
{
	YCLockGuard guard(&myMapLock);
	std::map<std::string, int>::iterator at = myDownloadMap.find(filename);
	if (at != myDownloadMap.end())
	{
		if (at->second == DOWNLOADING)
		{
			return false;    //
		}
		else if (at->second == SUCCESSED)
		{
			LOG_INFO("YCNetworkFileLoader::addToDownloadMap�ظ������Ѿ����سɹ����ļ���" << filename);
			return false;
		}
	}

	// 1. at->second == FAILED
	// 2. at == myDownloadMap.end()
	myDownloadMap[filename] = DOWNLOADING;
	return true;
}