#ifndef _INCLUDE_YCLOCK_H_
#define _INCLUDE_YCLOCK_H_

#include "YCILock.h"
#include "YCDef.h"

class YCAPI YCLock :
	public YCILock
{
public:
	
	YCLock(void);
	
	virtual ~YCLock(void);

	//
	// 函数：acquire()
	//
	// 目的：加锁函数
	//
	virtual void acquire();

	//
	// 函数：release()
	//
	// 目的：解锁函数
	//
	virtual void release();

private:

	CRITICAL_SECTION myMutex;
};

#endif

