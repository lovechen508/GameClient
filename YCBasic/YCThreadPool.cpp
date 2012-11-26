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
 * YCThreadPoolImpl :  线程池内部实现类，对接口屏蔽实现细节
 *                     避免DLL接口出现STL容器类
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
	// 函数：enqueue(WorkItem* item)
	//
	// 目的：向工作队列中插入工作项
	//
	void enqueue(YCThreadPool::WorkItem* item)
	{		
		if (item == NULL)
		{
			throw YCException(2002, "YCThreadPool::enqueue插入空工作项!");
		}

		if (isStopped())
		{
			throw YCException(2002, "YCThreadPool::enqueue线程池已停止!");
		}
	
		YCLockGuard guard(&myLock);

		while (!isStopped() && (myList.size() > myMaxItems))
		{
			myCondition.wait(&myLock, 1000); 
		}

		if (isStopped())
		{
			throw YCException(2002, "YCThreadPool::enqueue线程池已停止!");
		}

		myList.append(item, NULL); //放到队列末尾

		if (myList.size() == 1)
		{
			myCondition.notify();
		}
	}

	//
	// 函数：abort()
	//
	// 目的：停止线程池，放弃未完成的工作项
	//
	void abort()
	{		
		if (isStopped())
		{
			throw YCException(2002, "YCThreadPool::abort线程池已停止!");
	
		}
		
		stop();

		YCLockGuard guard(&myLock);

		myList.clear(threadpool_item_release);

		myCondition.notify();
	}

	//
	// 函数：finish()
	//
	// 目的：不再接受工作项的插入，此时调用enqueue将抛出YCException
	//      完成队列中所有工作项，停止线程池
	//
	void finish()
	{		
		if (isStopped())
		{
			throw YCException(2002, "YCThreadPool::stop线程池已停止!");
		}

		stop();

		myCondition.notify();
	}

	//
	// 函数：entries()
	//
	// 目的：返回等待队列中的工作项数量
	//
	int entries()
	{
		YCLockGuard guard(&myLock);
		return myList.size();
	}

	// 
	// 函数：run()
	//
	// 目的：线程实现函数
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
				LOG_WARNING("YCThreadPool::run工作项出现异常：" << e.code() << " " << e.what());
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
// 函数：enqueue(WorkItem* item)
//
// 目的：向工作队列中插入工作项
//
void YCThreadPool::enqueue(WorkItem* item)
{
	myThreadPool->enqueue(item);
}

//
// 函数：abort()
//
// 目的：停止线程池，放弃未完成的工作项
//
void YCThreadPool::abort()
{
	myThreadPool->abort();
}

//
// 函数：stop()
//
// 目的：不再接受工作项的插入，此时调用enqueue将抛出YCException
//      完成队列中所有工作项，停止线程池
//
void YCThreadPool::stop()
{
	myThreadPool->finish();
}

//
// 函数：entries()
//
// 目的：返回等待队列中的工作项数量
//
int YCThreadPool::entries()
{
	return myThreadPool->entries();
}
