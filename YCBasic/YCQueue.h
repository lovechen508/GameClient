#ifndef _INCLUDE_YCQUEUE_H_
#define _INCLUDE_YCQUEUE_H_

#include "YCDef.h"

class YCDList;

/*
 * YCQueue : �Ƚ��ȳ�����
 */
class YCAPI YCQueue 
{
public:

	//
	// ������YCQueue(int maxSize)
	//
	// Ŀ�ģ�����FIFO����
	//
	YCQueue();

	//
	// ������~YCQueue()
	//
	// Ŀ�ģ�����FIFO����
	//
	~YCQueue();

	//
	// ������size()
	//
	// Ŀ�ģ����г���
	//
	int size();

	//
	// ������push(void* item)
	//
	// Ŀ�ģ�����Item����β
	// 
	void push(void* item);

	//
	// ������front()
	//
	// Ŀ�ģ��鿴������Ԫ�أ�������
	// 
	void* front();

	//
	// ������pop()
	//
	// Ŀ�ģ�����������Ԫ��
	//
	void* pop();

	//
	// ������clear()
	//
	// Ŀ�ģ���յ�ǰ���У�Delete����item
	//       ���ڱ����payload����void*ָ�룬����delete����
	//       ���õ���Ӧ�������������ڵ�����������ǰ�������clear
	//       ��������ͷ��Ȼ����
	//
	// ע�ͣ�
	//       ���release == NULL, �������������payload���������������ڣ������ͷ�
	//       ���release != NULL, ���Զ����е�ÿһ�����release(payload)���ͷ���Դ
	void clear(void(*release)(void*) = NULL);

private:

	YCDList * myList;
};

#endif