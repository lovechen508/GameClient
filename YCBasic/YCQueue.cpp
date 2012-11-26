#include "YCQueue.h"

#include "YCAssert.h"
#include "YCDef.h"
#include "YCDList.h"

//
// ������YCQueue(int maxSize)
//
// Ŀ�ģ�����FIFO����
//
YCQueue::YCQueue()
	: myList(new YCDList())
{

}

//
// ������~YCQueue()
//
// Ŀ�ģ�����FIFO����
//
YCQueue::~YCQueue()
{
	delete myList;
}

//
// ������size()
//
// Ŀ�ģ����г���
//
int YCQueue::size()
{
	return myList->size();
}

//
// ������push(void* item)
//
// Ŀ�ģ�����Item����β
// 
void YCQueue::push(void* item)
{
	myList->append(item, NULL);
}

//
// ������front()
//
// Ŀ�ģ��鿴������Ԫ�أ�������
// 
void* YCQueue::front()
{
	Item_List_T* item = myList->begin();
	if (item != NULL)
	{
		return myList->payload(item);
	}
	return NULL;
}

//
// ������pop()
//
// Ŀ�ģ�����������Ԫ��
//
void* YCQueue::pop()
{
	Item_List_T* item = myList->begin();
	if (item != NULL)
	{
		return myList->remove(item);
	}
	return NULL;
}

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
void YCQueue::clear(void(*release)(void*))
{
	myList->clear(release);
}