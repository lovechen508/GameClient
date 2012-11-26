#include "YCThreadPool.h"

#include "YCDef.h"
#include "YCLock.h"
#include "YCAssert.h"
#include "YCThread.h"
#include "YCDList.h"
#include "YCLogger.h"
#include "YCException.h"
#include "YCIRunnable.h"
#include "YCCondition.h"
#include "YCLockGuard.h"

static void threadpool_item_release(void* item)
{
	YCThreadPool::WorkItem* workItem = (YCThreadPool::WorkItem*)item;
	SAFE_DELETE(workItem);
}

/*
 * YCThreadPoolImpl :  �̳߳��ڲ�ʵ���࣬�Խӿ�����ʵ��ϸ��
 *                     ����DLL�ӿڳ���STL������
 */
class YCThreadPoolImpl : public YCIRunnable
{

public:

	YCThreadPoolImpl(int noOfThreads, int maxItems)
		: myNoOfThreads(noOfThreads)
		, myMaxItems(maxItems)
	{
		
	}

	void initialize()
	{
		myThreads = new YCThread*[myNoOfThreads];
		for (int i = 0; i < myNoOfThreads; ++i)
		{
			myThreads[i] = new YCThread(this);
			myThreads[i]->start(YCThread::NORMAL);
		}
	}

	void finalize()
	{
		for (int i = 0; i < myNoOfThreads; ++i)
		{
			myThreads[i]->join();
			SAFE_DELETE(myThreads[i]);
		}
		SAFE_DELETE_ARRAY(myThreads);
	}

	~YCThreadPoolImpl()
	{
	}

	//
	// ������enqueue(WorkItem* item)
	//
	// Ŀ�ģ����������в��빤����
	//
	void enqueue(YCThreadPool::WorkItem* item)
	{		
		if (item == NULL)
		{
			throw YCException(2002, "YCThreadPool::enqueue����չ�����!");
		}

		if (isStopped())
		{
			throw YCException(2002, "YCThreadPool::enqueue�̳߳���ֹͣ!");
		}
	
		YCLockGuard guard(&myLock);

		while (!isStopped() && (myList.size() > myMaxItems))
		{
			myCondition.wait(&myLock, 1000); 
		}

		if (isStopped())
		{
			throw YCException(2002, "YCThreadPool::enqueue�̳߳���ֹͣ!");
		}

		myList.append(item, NULL); //�ŵ�����ĩβ

		if (myList.size() == 1)
		{
			myCondition.notify();
		}
	}

	//
	// ������abort()
	//
	// Ŀ�ģ�ֹͣ�̳߳أ�����δ��ɵĹ�����
	//
	void abort()
	{		
		if (isStopped())
		{
			throw YCException(2002, "YCThreadPool::abort�̳߳���ֹͣ!");
	
		}
		
		stop();

		YCLockGuard guard(&myLock);

		myList.clear(threadpool_item_release);

		myCondition.notify();
	}

	//
	// ������finish()
	//
	// Ŀ�ģ����ٽ��ܹ�����Ĳ��룬��ʱ����enqueue���׳�YCException
	//      ��ɶ��������й����ֹͣ�̳߳�
	//
	void finish()
	{		
		if (isStopped())
		{
			throw YCException(2002, "YCThreadPool::stop�̳߳���ֹͣ!");
		}

		stop();

		myCondition.notify();
	}

	//
	// ������entries()
	//
	// Ŀ�ģ����صȴ������еĹ���������
	//
	int entries()
	{
		YCLockGuard guard(&myLock);
		return myList.size();
	}

	// 
	// ������run()
	//
	// Ŀ�ģ��߳�ʵ�ֺ���
	//
	int run()
	{
		while (!isStopped())
		{
			YCThreadPool::WorkItem* workItem = acquireJob();
			if (workItem == NULL) break;
			try
			{
				workItem->run();
				delete workItem;
			}
			catch (YCException& e)
			{
				LOG_WARNING("YCThreadPool::run����������쳣��" << e.code() << " " << e.what());
			}
		}
		return 0;
	}

private:

	YCThreadPool::WorkItem* acquireJob()
	{
		YCLockGuard guard(&myLock);
		while (myList.size() == 0)
		{
			if (isStopped())
			{
				return NULL;
			}

			myCondition.wait(&myLock, 1000);
		}

		YCThreadPool::WorkItem* workItem = NULL;

		Item_List_T* item = myList.begin();
		if (item != NULL)
		{
			(YCThreadPool::WorkItem*)myList.remove(item);
			if (myList.size() == (myMaxItems-1))
			{
				myCondition.notify();
			}
		}
		return workItem;
	}

	int myMaxItems;

	int myNoOfThreads;
	YCThread** myThreads;

	YCDList myList;
	YCLock myLock;
	YCCondition myCondition;

};

YCThreadPool::YCThreadPool(int noOfThreads, int maxItems)
{
	myThreadPool = new YCThreadPoolImpl(noOfThreads, maxItems);
	myThreadPool->initialize();
}


YCThreadPool::~YCThreadPool(void)
{
	myThreadPool->finalize();
	delete myThreadPool;
}

//
// ������enqueue(WorkItem* item)
//
// Ŀ�ģ����������в��빤����
//
void YCThreadPool::enqueue(WorkItem* item)
{
	myThreadPool->enqueue(item);
}

//
// ������abort()
//
// Ŀ�ģ�ֹͣ�̳߳أ�����δ��ɵĹ�����
//
void YCThreadPool::abort()
{
	myThreadPool->abort();
}

//
// ������stop()
//
// Ŀ�ģ����ٽ��ܹ�����Ĳ��룬��ʱ����enqueue���׳�YCException
//      ��ɶ��������й����ֹͣ�̳߳�
//
void YCThreadPool::stop()
{
	myThreadPool->finish();
}

//
// ������entries()
//
// Ŀ�ģ����صȴ������еĹ���������
//
int YCThreadPool::entries()
{
	return myThreadPool->entries();
}
