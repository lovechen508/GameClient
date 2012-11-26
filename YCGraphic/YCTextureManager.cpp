#include "YCTextureManager.h"

#include "YCBasic\YCAssert.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLock.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCLockGuard.h"
#include "YCBasic\YCSharedPtr.h"
#include "YCBasic\YCCondition.h"
#include "YCBasic\YCException.h"

#include "YCConfig\YCConfig.h"

#include "YCTexture.h"
#include "YCITexture2D.h"
#include "YCTextureIDUtil.h"
#include "YCTextureFactory.h"

#pragma region YCTexturePoolImpl

#include <list>
#include <map>

typedef std::list<YCITexture2D*> YCTexture2DList;
typedef std::map<E_TEXTURE_TYPE, YCTexture2DList> YCTexture2DMap;

class YCTexturePoolImpl
{
public:

	YCTexturePoolImpl() {}

	~YCTexturePoolImpl()
	{
	}

	void release()
	{
		for (YCTexture2DMap::iterator it = myTextureMapList.begin();
			 it != myTextureMapList.end();
			 ++it)
		{
			YCTexture2DList& list = it->second;
			for (YCTexture2DList::iterator it2 = list.begin(); it2 != list.end(); ++it2)
			{
				YCITexture2D* texture2D = (YCITexture2D*)*it2;
				if (texture2D->referenceCount() > 0)
				{
					throw YCException(2002, "YCTexturePoolImpl::release释放纹理池时纹理计数大于0！");
				}
				else
				{
					SAFE_DELETE(texture2D);
				}
			}
		}
	}

	YCITexture2D* fetch(int uniqueId)
	{
		E_TEXTURE_TYPE type = (E_TEXTURE_TYPE)GET_TEXTURE_TYPE(uniqueId);
		YCLockGuard guard(&myLock);
		YCTexture2DMap::iterator at = myTextureMapList.find(type);
		if (at == myTextureMapList.end())
		{
			return NULL;
		}

		YCTexture2DList& list = at->second;
		for (YCTexture2DList::iterator it = list.begin(); it != list.end(); ++it)
		{
			YCITexture2D* texture2D = (YCITexture2D*)*it;
			if (texture2D->hasTexture(uniqueId))
			{
				return texture2D;
			}
		}

		return NULL;
	}

	void insert(E_TEXTURE_TYPE type, YCITexture2D* texture)
	{
		YCLockGuard guard(&myLock);
		YCTexture2DMap::iterator at = myTextureMapList.find(type);
		if (at == myTextureMapList.end())
		{
			LOG_INFO("YCTextureManager::insert开辟新纹理池类型：" << (int)type);
		}

		myTextureMapList[type].push_front(texture);
	}

	void shrink(E_TEXTURE_TYPE type, int duraSeconds)
	{
		YCLockGuard guard(&myLock);
		YCTexture2DMap::iterator at = myTextureMapList.find(type);
		if (at == myTextureMapList.end())
		{
			LOG_INFO("YCTextureManager::shrink纹理池类型不存在：" << (int)type);
		}
		else
		{

		}
	}

	void purge(E_TEXTURE_TYPE type)
	{
		YCLockGuard guard(&myLock);
		YCTexture2DMap::iterator at = myTextureMapList.find(type);
		if (at == myTextureMapList.end())
		{
			LOG_INFO("YCTextureManager::purge纹理池类型不存在：" << type);
		}
		else
		{

		}
	}

private:

	YCLock myLock;
	YCTexture2DMap myTextureMapList;
};

#pragma endregion YCTexturePoolImpl

YCTextureManager::YCTextureManager()
	: myStatus(STOPPED)
	, myTexturePool(NULL)
	, myLRUThread(NULL)
	, myLRUCondition(NULL)
	, myFactory(NULL)
{
	//initialize 中完成创建
	memset(&myScheduledItem, 0, sizeof(scheduledItem));
}

YCTextureManager::~YCTextureManager()
{
	//finalize 中完成资源释放
}

//
// 函数：initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
//
// 目的：初始化Http网络连接池  
//       初始化内部数据结构   
//       启动LRU监视线程
//
bool YCTextureManager::initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
{
	SMART_ASSERT(networkLoader != NULL);
	SMART_ASSERT(myTexturePool == NULL);

	myDurationSeconds = config.durationSeconds();

	// 初始化Http网络连接池
	myFactory = new YCTextureFactory();
	if (!myFactory->initialize(this, networkLoader, config))
	{
		return false;
	}

	// 初始化内部数据结构
	myTexturePool = new YCTexturePoolImpl();

	// 资源监视线程
	myLRUCondition = new YCCondition();
	myLRUThread = new YCThread(this);
	myLRUThread->start(YCThread::LOW);

	myStatus = STARTED;

	return true;
}

//
// 函数：fetch(int poolNo, int uniqueId, const chra* filename)
//
// 目的：获取纹理
//      客户唯一访问纹理接口
//
// 注释：
//      客户必须保证纹理类型的正确和uniqueId的唯一性，用于本地缓存查找
//      如果本地缓存命中，直接返回对应纹理
//      否则，请求YCTextureFactory获取纹理
//		    YCTextureFactory:   如果对应目录文件尚未下载，返回YCTexture2DFuture,同时向YCINetworkLoader提交下载请求
//                              如果对应目录下文件存在，读入并构建YCSingleTexture2D
//                              如果对应目录下存在TextureManager生成的XML Altas文件，读入并构建YCAtlasTexture2D
//
YCTexture* YCTextureManager::fetch(E_TEXTURE_TYPE type, const char* filename)
{
	if (myStatus == STOPPED)
	{
		throw YCException(2002, "YCTextureManager::fetch纹理池未启动!");
	}

	unsigned int uniqueId = YCTextureIDUtil::encode(type, filename);	

	// 查看缓存是否命中
	YCITexture2D* texture2D = myTexturePool->fetch(uniqueId);
	if (texture2D == NULL)
	{
		// 文件加载请求，有可能引起网络请求
		texture2D = myFactory->fetch(type, uniqueId, filename);		
		if (texture2D != NULL)
		{
			//插入缓存池
			myTexturePool->insert(type, texture2D); 
		}
	}

	YCTexture* result = NULL;
	if ( (texture2D != NULL) && (texture2D->status() == YCITexture2D::SUCC) )
	{
		result = new YCTexture(uniqueId);
		result->bind(texture2D); //增加引用计数
	}

	return result;
}

//
// 函数：update(E_TEXTURE_TYPE type, YCITexture2D* texture);
//
// 目的：用于YCTextureFactory下载，加载纹理成功后
//      通知纹理池更新纹理状态
//
void YCTextureManager::update(E_TEXTURE_TYPE type, YCITexture2D* texture)
{
	if (myStatus == STOPPED)
	{
		throw YCException(2002, "YCTextureManager::update纹理池未启动!");
	}
	
	// 直接插入队列的最前面
	// 那些FAKE的纹理等待清理线程去删除
	myTexturePool->insert(type, texture);
}

//
// 函数：shrink(int poolId)
//
// 目的：收缩纹理池
//
// 注释：
//      如果 poolId <> 0, 收缩对应的纹理池
//      如果 poolId == 0, 收缩所有的纹理池
//
void YCTextureManager::shrink(int poolId)
{
	myScheduledItem.actionType = 1;
	myScheduledItem.parameter = poolId;

	myLRUCondition->notify();
}

// 
// 函数: purge(int poolId)
//
// 目的：清空纹理池
//
// 注释：
//      如果 poolId <> 0, 清空对应的纹理池
//      如果 poolId == 0, 清空所有的纹理池
//
void YCTextureManager::purge(int poolId)
{
	myScheduledItem.actionType = 2;
	myScheduledItem.parameter = poolId;

	myLRUCondition->notify();
}

//
// 函数：finalize()
//
// 目的：停止LRU监视线程   
//       释放Http网络连接池  
//       释放内部数据结构
//
void YCTextureManager::finalize()
{
	if (myStatus == STOPPED)
	{
		return;
	}

	// 停止LRU监视线程
	myLRUThread->join();
	delete myLRUThread;
	delete myLRUCondition;

	// 释放Http网络连接池

	// 释放内部数据结构
	try
	{
		myTexturePool->release();
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCTextureManager::finalize释放纹理池异常：" << e.what());
	}
	delete myTexturePool;

	myStatus = STOPPED;
}

//
// 函数：dump()
//
// 目的：dump类内部状态为Html片段
//
const char* YCTextureManager::dump()
{
	return NULL;
}

static time_t __lastrun = 0;
// 
// 函数：run()
//
// 目的：监视线程实现函数
//
int YCTextureManager::run()
{
	try
	{
		while (!isStopped())
		{
			time_t now = GetTickCount();
			if ( (now - __lastrun) > (myDurationSeconds*1000) )
			{
				LOG_DEBUG("YCTextureManager::run开始执行!");

				if      (myScheduledItem.actionType == 0)
				{
				}
				else if (myScheduledItem.actionType == 1)
				{
				}
				else if (myScheduledItem.actionType == 2)
				{
				}
				memset(&myScheduledItem, 0, sizeof(myScheduledItem));
				__lastrun = now;
			}
			myLRUCondition->wait(2000);
		}
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCTextureManager::run清理线程异常退出:" << e.what());
		return -1;
	}

	return 0;
}