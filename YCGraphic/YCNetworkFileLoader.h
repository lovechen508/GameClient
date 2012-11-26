#ifndef _INCLUDE_YCNETWORKFILELOADER_H_
#define _INCLUDE_YCNETWORKFILELOADER_H_

#include <map>

#include "YCBasic\YCLock.h"
#include "YCBasic\YCThreadPool.h"

class YCConfig;
class YCNetworkFileLoadListener;

/*
 * YCNetworkFileLoader : 网络远程文件纹理加载器
 */
class YCNetworkFileLoader 
{
	
	enum E_DOWNLOAD_STATUS {DOWNLOADING = 0, SUCCESSED = 1, FAILED = 2};

public:

	YCNetworkFileLoader();
	
	virtual ~YCNetworkFileLoader(void);
		
	//
	// 函数：initialize()
	//
	// 目的：加载纹理相关配置
	//
	bool initialize(YCConfig& config);

	//
	// 函数：request(YCNetworkFileLoaderListener* listener, const char* filename)
	//
	// 目的：网络下载实现函数
	//
	// 参数：filename为完整URL
	//
	virtual void request(YCNetworkFileLoadListener* listener, const char* filename);

	//
	// 函数：finalize()
	//
	// 目的：释放Http网络连接池   
	//       释放内部数据结构
	//
	void finalize();

private:

	//
	// 函数：addToDownloadMap(const char* filename)
	//
	// 目的：添加到下载列表中,防止重复下载
	//
	bool addToDownloadMap(const char* filename);

private:

	int myNoOfConnections;

	std::string myDownloadServerURL;
	
	//
	// 属性：myDownloadMap
	//
	// Key：下载文件名
	// Value：下载状态 0, 正在下载 1, 下载成功
	//        如果下载失败，会从map中移除。等YCTextureManager中的失效后重新启动下载
	std::map<std::string, int> myDownloadMap;

	YCLock myMapLock;
		
	YCThreadPool* myHttpClients;
};

#endif

