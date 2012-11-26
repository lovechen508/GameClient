#ifndef _INCLUDE_YCILOCK_H_
#define _INCLUDE_YCILOCK_H_

class YCAPI YCILock
{
public:
	YCILock(void);

	virtual ~YCILock(void);

	//
	// ������acquire()
	//
	// Ŀ�ģ���������
	//
	virtual void acquire() = 0;

	//
	// ������release()
	//
	// Ŀ�ģ���������
	//
	virtual void release() = 0;
};

#endif

