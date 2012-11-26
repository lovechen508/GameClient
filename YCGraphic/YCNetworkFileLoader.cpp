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
	// 函数：void run();
	//
	// 目的：完成每个工作项的实际内容
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
// 函数：configure()
//
// 目的：加载纹理相关配置
//
bool YCNetworkFileLoader::initialize(YCConfig& config)
{
	SMART_ASSERT(myHttpClients == NULL);

	myDownloadServerURL = config.downloadServerURL();
	myNoOfConnections = config.noOfConnections();
	int maxItemsInQueue = config.maxItemsInQueue();
	
	if (myNoOfConnections == 0)
	{
		LOG_WARNING("纹理最大下载链接数为0，关闭网络下载功能!");
	}
	else
	{
		myNoOfConnections = min(max(myNoOfConnections, 1), 5);
		int realMaxItemsInQueue = min(max(maxItemsInQueue, 10), 100);
		LOG_INFO("纹理最大并发下载连接数：" << myNoOfConnections);
		LOG_INFO("纹理最大等待队列：" << realMaxItemsInQueue);
		myHttpClients = new YCThreadPool(myNoOfConnections, realMaxItemsInQueue);
	}

	return true;
}

//
// 函数：request(const char* filename)
//
// 目的：网络下载实现函数
//
// 参数：filename为完整URL
//
void YCNetworkFileLoader::request(YCNetworkFileLoadListener* listener, const char* filename)
{
	if (listener == NULL)
	{
		throw YCException(2002, "YCNetworkFileLoader::createWorkItem中listener为NULL");
	}

	if (filename == NULL)
	{
		throw YCException(2002, "YCNetworkFileLoader::createWorkItem中filename为NULL");
	}

	if (myNoOfConnections == 0 || myHttpClients == NULL)
	{
		return; //网络加载关闭
	}

	if (addToDownloadMap(filename))
	{
		myHttpClients->enqueue(new NetworkWorkItem(myDownloadServerURL.c_str(), filename, listener));
	}
}

//
// 函数：finalize()
//
// 目的：释放Http网络连接池   
//       释放内部数据结构
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
// 函数：addToDownloadMap(const char* filename)
//
// 目的：添加到下载列表中,防止重复下载
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
			LOG_INFO("YCNetworkFileLoader::addToDownloadMap重复下载已经下载成功的文件：" << filename);
			return false;
		}
	}

	// 1. at->second == FAILED
	// 2. at == myDownloadMap.end()
	myDownloadMap[filename] = DOWNLOADING;
	return true;
}