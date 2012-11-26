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
// ������wait(unsigned int dwMilliseconds)
//
// Ŀ�ģ��ͷ������ȴ�notify������ǰ���»����
//
void YCCondition::wait(unsigned int dwMilliseconds)
{
	WaitForSingleObject(myEvent, dwMilliseconds);
}

//
// ������wait(YCILock* lock, unsigned int dwMilliseconds)
//
// Ŀ�ģ��ͷ������ȴ�notify������ǰ���»����
//
void YCCondition::wait(YCILock* lock, unsigned int dwMilliseconds)
{
	SMART_ASSERT(lock != NULL);
	lock->release();
	WaitForSingleObject(myEvent, dwMilliseconds);
	lock->acquire();
}

//
// ������notify()
//
// Ŀ�ģ�֪ͨ
//
void YCCondition::notify()
{
	SetEvent(myEvent);
}
