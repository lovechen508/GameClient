#ifndef _INCLUDE_YCLOCKGUARD_H_
#define _INCLUDE_YCLOCKGUARD_H_

class YCILock;

/*
 * YCLockGuard: YCILock�ӽ���������
 *
 * ע�ͣ��ڹ��캯���м���
 *       �����������н���
 */
class YCAPI YCLockGuard
{
public:

	//
	// ������YCLockGuard(YCILock* lock)
	//
	// Ŀ�ģ�����
	//
	YCLockGuard(YCILock* lock);

	//
	// ������~YCLockGuard(void)
	//
	// Ŀ�ģ�����
	//
	~YCLockGuard(void);

private:

	YCILock *myLock;
};

#endif

