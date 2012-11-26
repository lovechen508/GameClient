#ifndef _INCLUDE_YCCONDITION_H_
#define _INCLUDE_YCCONDITION_H_

#include "YCDef.h"

class YCILock;

/*
 * YCCondition : ��������������
 *
 * ע�ͣ�
 */
class YCAPI YCCondition
{
public:
	
	YCCondition(void);
	
	~YCCondition(void);

	//
	// ������wait(unsigned int dwMilliseconds)
	//
	// Ŀ�ģ��ͷ������ȴ�notify������ǰ���»����
	//
	void wait(unsigned int dwMilliseconds = INFINITE);

	//
	// ������wait(YCILock* lock, unsigned int dwMilliseconds)
	//
	// Ŀ�ģ��ͷ������ȴ�notify������ǰ���»����
	//
	void wait(YCILock* lock, unsigned int dwMilliseconds = INFINITE);

	//
	// ������notify()
	//
	// Ŀ�ģ�֪ͨ�����߳�
	//
	void notify();

private:

	HANDLE myEvent;

};

#endif

