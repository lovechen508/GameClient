#include "YCLockGuard.h"

#include "YCDef.h"
#include "YCILock.h"

#include "YCAssert.h"

YCLockGuard::YCLockGuard(YCILock* lock)
	: myLock(lock)
{
	SMART_ASSERT(lock != NULL);
	myLock->acquire();
}


YCLockGuard::~YCLockGuard(void)
{
	myLock->release();
}
