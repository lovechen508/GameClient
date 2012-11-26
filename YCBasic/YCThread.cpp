#include "YCThread.h"

#include "YCDef.h"
#include "YCLogger.h"
#include "YCIRunnable.h"
#include "YCException.h"

#include "YCAssert.h"

extern "C" DWORD WINAPI run_impl(void *arg);

/*
 * YCThreadImpl : �߳��ڲ�ʵ�ֺ���
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
			throw YCException(2002, "YCThread����Threadʧ��!");
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
			LOG_FATAL("YCThread::run�����쳣���߳��˳���" << e.what());
			myRunnable->stop();
			//�ж��̣߳��˳�
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
		throw YCException(2002, "YCThread����ʧ�ܣ�runnableΪ��!");
	}
	myImpl = new YCThreadImpl(runnable);
}


YCThread::~YCThread(void)
{
	delete myImpl;
}

//
// ������start()
//
// Ŀ�ģ������߳�
//
void YCThread::start(YC_THREAD_PRIORITY priority)
{
	myImpl->start(priority);
}

//
// ������join()
//
// Ŀ�ģ�����֪ͨ���ȴ��߳̽���
//
void YCThread::join()
{
	myImpl->join();
}

///////////////////////////////////////////////////////
// run_impl ���̰߳�װ����
///////////////////////////////////////////////////////
extern "C" DWORD WINAPI run_impl(void *arg)
{
	return ((YCThreadImpl*)arg)->run();
}
