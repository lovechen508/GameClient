#ifndef _INCLUDE_YCRWLOCK_H_
#define _INCLUDE_YCRWLOCK_H_

/*
 * YCRWLock : ��д��
 *
 * ע�ͣ�֧�����ּ�����ʽ
 *       ������д��
 *       ��������Ϊд�����������ͷŶ������ٻ�ȡд��
 */
class YCAPI YCRWLock
{
public:
	
	YCRWLock(void);
	
	~YCRWLock(void);

	//
	// ������read()
	//
	// Ŀ�ģ�ֻ����������
	//
	virtual void read();

	//
	// ������write()
	//
	// Ŀ�ģ�ֻ����������
	//
	virtual void write();

	//
	// ������release()
	//
	// Ŀ�ģ���������
	//
	virtual void release();
};

#endif

