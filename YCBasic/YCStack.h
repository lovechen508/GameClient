#ifndef _INCLUDE_YCSTACK_H_
#define _INCLUDE_YCSTACK_H_

class YCDList;

class YCAPI YCStack
{
public:

	YCStack(void);

	virtual ~YCStack(void);

	//
	// ������push(void * payload)
	//
	// Ŀ�ģ�ѹջ
	//
	void push(void * payload);

	//
	// ������front()
	//
	// Ŀ�ģ�����ջ��Ԫ�أ�������
	//
	void* front();

	//
	// ������pop()
	//
	// Ŀ�ģ�����������ջ��Ԫ��
	//
	void* pop();
	
private:

	YCDList * myList;
};

#endif