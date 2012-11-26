#include "YCCondition.h"

#include "YCAssert.h"
#include "YCILock.h"

YCCondition::YCCondition(void)
{
	myEvent = CreateEvent(NULL, false, false, NULL);
}


YCCondition::~YCCondition(void)
{
	CloseHandle(myEvent);
}

//
// 函数：wait(unsigned int dwMilliseconds)
//
// 目的：释放锁，等待notify，返回前重新获得锁
//
void YCCondition::wait(unsigned int dwMilliseconds)
{
	WaitForSingleObject(myEvent, dwMilliseconds);
}

//
// 函数：wait(YCILock* lock, unsigned int dwMilliseconds)
//
// 目的：释放锁，等待notify，返回前重新获得锁
//
void YCCondition::wait(YCILock* lock, unsigned int dwMilliseconds)
{
	SMART_ASSERT(lock != NULL);
	lock->release();
	WaitForSingleObject(myEvent, dwMilliseconds);
	lock->acquire();
}

//
// 函数：notify()
//
// 目的：通知
//
void YCCondition::notify()
{
	SetEvent(myEvent);
}
