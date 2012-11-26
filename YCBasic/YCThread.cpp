#include "YCThread.h"

#include "YCDef.h"
#include "YCLogger.h"
#include "YCIRunnable.h"
#include "YCException.h"

#include "YCAssert.h"

extern "C" DWORD WINAPI run_impl(void *arg);

/*
 * YCThreadImpl : 线程内部实现函数
 */
class YCThreadImpl
{
public:

	YCThreadImpl(YCIRunnable* r)
		: myRunnable(r)
	{
	}

	~YCThreadImpl()
	{
	}

	void start(YCThread::YC_THREAD_PRIORITY priority)
	{
		myThread = ::CreateThread(NULL, NULL, run_impl, this, 0, &myThreadId);
		if (myThread == NULL)
		{
			throw YCException(2002, "YCThread创建Thread失败!");
		}

		if      (priority == YCThread::LOW)    SetThreadPriority(myThread, THREAD_PRIORITY_LOWEST);
		else if (priority == YCThread::NORMAL) SetThreadPriority(myThread, THREAD_PRIORITY_NORMAL);
		else if (priority == YCThread::HIGH)   SetThreadPriority(myThread, THREAD_PRIORITY_HIGHEST);
	}

	DWORD run()
	{
		DWORD exitCode;
		try 
		{
			exitCode = myRunnable->run();
		}
		catch(YCException& e)
		{
			LOG_FATAL("YCThread::run出现异常，线程退出：" << e.what());
			myRunnable->stop();
			//中断线程，退出
		}

		::ExitThread(exitCode);
	}

	void join()
	{
		myRunnable->stop();
		WaitForSingleObject(myThread, INFINITE);
	}

private:
	YCIRunnable* myRunnable;
	DWORD myThreadId;
	HANDLE myThread;
};

YCThread::YCThread(YCIRunnable* runnable)
{
	SMART_ASSERT(runnable != NULL);
	if (runnable == NULL)
	{
		throw YCException(2002, "YCThread创建失败，runnable为空!");
	}
	myImpl = new YCThreadImpl(runnable);
}


YCThread::~YCThread(void)
{
	delete myImpl;
}

//
// 函数：start()
//
// 目的：启动线程
//
void YCThread::start(YC_THREAD_PRIORITY priority)
{
	myImpl->start(priority);
}

//
// 函数：join()
//
// 目的：发送通知并等待线程结束
//
void YCThread::join()
{
	myImpl->join();
}

///////////////////////////////////////////////////////
// run_impl ：线程包装函数
///////////////////////////////////////////////////////
extern "C" DWORD WINAPI run_impl(void *arg)
{
	return ((YCThreadImpl*)arg)->run();
}
