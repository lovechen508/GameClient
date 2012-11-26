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
	// ������acquire()
	//
	// Ŀ�ģ���������
	//
	virtual void acquire();

	//
	// ������release()
	//
	// Ŀ�ģ���������
	//
	virtual void release();

private:

	CRITICAL_SECTION myMutex;
};

#endif

