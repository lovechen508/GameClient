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
					throw YCException(2002, "YCTexturePoolImpl::release�ͷ������ʱ�����������0��");
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
			LOG_INFO("YCTextureManager::insert��������������ͣ�" << (int)type);
		}

		myTextureMapList[type].push_front(texture);
	}

	void shrink(E_TEXTURE_TYPE type, int duraSeconds)
	{
		YCLockGuard guard(&myLock);
		YCTexture2DMap::iterator at = myTextureMapList.find(type);
		if (at == myTextureMapList.end())
		{
			LOG_INFO("YCTextureManager::shrink��������Ͳ����ڣ�" << (int)type);
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
			LOG_INFO("YCTextureManager::purge��������Ͳ����ڣ�" << type);
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
	//initialize ����ɴ���
	memset(&myScheduledItem, 0, sizeof(scheduledItem));
}

YCTextureManager::~YCTextureManager()
{
	//finalize �������Դ�ͷ�
}

//
// ������initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
//
// Ŀ�ģ���ʼ��Http�������ӳ�  
//       ��ʼ���ڲ����ݽṹ   
//       ����LRU�����߳�
//
bool YCTextureManager::initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
{
	SMART_ASSERT(networkLoader != NULL);
	SMART_ASSERT(myTexturePool == NULL);

	myDurationSeconds = config.durationSeconds();

	// ��ʼ��Http�������ӳ�
	myFactory = new YCTextureFactory();
	if (!myFactory->initialize(this, networkLoader, config))
	{
		return false;
	}

	// ��ʼ���ڲ����ݽṹ
	myTexturePool = new YCTexturePoolImpl();

	// ��Դ�����߳�
	myLRUCondition = new YCCondition();
	myLRUThread = new YCThread(this);
	myLRUThread->start(YCThread::LOW);

	myStatus = STARTED;

	return true;
}

//
// ������fetch(int poolNo, int uniqueId, const chra* filename)
//
// Ŀ�ģ���ȡ����
//      �ͻ�Ψһ��������ӿ�
//
// ע�ͣ�
//      �ͻ����뱣֤�������͵���ȷ��uniqueId��Ψһ�ԣ����ڱ��ػ������
//      ������ػ������У�ֱ�ӷ��ض�Ӧ����
//      ��������YCTextureFactory��ȡ����
//		    YCTextureFactory:   �����ӦĿ¼�ļ���δ���أ�����YCTexture2DFuture,ͬʱ��YCINetworkLoader�ύ��������
//                              �����ӦĿ¼���ļ����ڣ����벢����YCSingleTexture2D
//                              �����ӦĿ¼�´���TextureManager���ɵ�XML Altas�ļ������벢����YCAtlasTexture2D
//
YCTexture* YCTextureManager::fetch(E_TEXTURE_TYPE type, const char* filename)
{
	if (myStatus == STOPPED)
	{
		throw YCException(2002, "YCTextureManager::fetch�����δ����!");
	}

	unsigned int uniqueId = YCTextureIDUtil::encode(type, filename);	

	// �鿴�����Ƿ�����
	YCITexture2D* texture2D = myTexturePool->fetch(uniqueId);
	if (texture2D == NULL)
	{
		// �ļ����������п���������������
		texture2D = myFactory->fetch(type, uniqueId, filename);		
		if (texture2D != NULL)
		{
			//���뻺���
			myTexturePool->insert(type, texture2D); 
		}
	}

	YCTexture* result = NULL;
	if ( (texture2D != NULL) && (texture2D->status() == YCITexture2D::SUCC) )
	{
		result = new YCTexture(uniqueId);
		result->bind(texture2D); //�������ü���
	}

	return result;
}

//
// ������update(E_TEXTURE_TYPE type, YCITexture2D* texture);
//
// Ŀ�ģ�����YCTextureFactory���أ���������ɹ���
//      ֪ͨ����ظ�������״̬
//
void YCTextureManager::update(E_TEXTURE_TYPE type, YCITexture2D* texture)
{
	if (myStatus == STOPPED)
	{
		throw YCException(2002, "YCTextureManager::update�����δ����!");
	}
	
	// ֱ�Ӳ�����е���ǰ��
	// ��ЩFAKE������ȴ������߳�ȥɾ��
	myTexturePool->insert(type, texture);
}

//
// ������shrink(int poolId)
//
// Ŀ�ģ����������
//
// ע�ͣ�
//      ��� poolId <> 0, ������Ӧ�������
//      ��� poolId == 0, �������е������
//
void YCTextureManager::shrink(int poolId)
{
	myScheduledItem.actionType = 1;
	myScheduledItem.parameter = poolId;

	myLRUCondition->notify();
}

// 
// ����: purge(int poolId)
//
// Ŀ�ģ���������
//
// ע�ͣ�
//      ��� poolId <> 0, ��ն�Ӧ�������
//      ��� poolId == 0, ������е������
//
void YCTextureManager::purge(int poolId)
{
	myScheduledItem.actionType = 2;
	myScheduledItem.parameter = poolId;

	myLRUCondition->notify();
}

//
// ������finalize()
//
// Ŀ�ģ�ֹͣLRU�����߳�   
//       �ͷ�Http�������ӳ�  
//       �ͷ��ڲ����ݽṹ
//
void YCTextureManager::finalize()
{
	if (myStatus == STOPPED)
	{
		return;
	}

	// ֹͣLRU�����߳�
	myLRUThread->join();
	delete myLRUThread;
	delete myLRUCondition;

	// �ͷ�Http�������ӳ�

	// �ͷ��ڲ����ݽṹ
	try
	{
		myTexturePool->release();
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCTextureManager::finalize�ͷ�������쳣��" << e.what());
	}
	delete myTexturePool;

	myStatus = STOPPED;
}

//
// ������dump()
//
// Ŀ�ģ�dump���ڲ�״̬ΪHtmlƬ��
//
const char* YCTextureManager::dump()
{
	return NULL;
}

static time_t __lastrun = 0;
// 
// ������run()
//
// Ŀ�ģ������߳�ʵ�ֺ���
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
				LOG_DEBUG("YCTextureManager::run��ʼִ��!");

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
		LOG_WARNING("YCTextureManager::run�����߳��쳣�˳�:" << e.what());
		return -1;
	}

	return 0;
}