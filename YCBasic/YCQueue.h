#ifndef _INCLUDE_YCQUEUE_H_
#define _INCLUDE_YCQUEUE_H_

#include "YCDef.h"

class YCDList;

/*
 * YCQueue : 先进先出队列
 */
class YCAPI YCQueue 
{
public:

	//
	// 函数：YCQueue(int maxSize)
	//
	// 目的：构建FIFO队列
	//
	YCQueue();

	//
	// 函数：~YCQueue()
	//
	// 目的：析构FIFO队列
	//
	~YCQueue();

	//
	// 函数：size()
	//
	// 目的：队列长度
	//
	int size();

	//
	// 函数：push(void* item)
	//
	// 目的：插入Item到队尾
	// 
	void push(void* item);

	//
	// 函数：front()
	//
	// 目的：查看队列首元素，不弹出
	// 
	void* front();

	//
	// 函数：pop()
	//
	// 目的：弹出队列首元素
	//
	void* pop();

	//
	// 函数：clear()
	//
	// 目的：清空当前队列，Delete所有item
	//       由于保存的payload都是void*指针，所以delete不会
	//       调用到对应的析构函数，在调用析构函数前必须调用clear
	//       但是链表头仍然保留
	//
	// 注释：
	//       如果release == NULL, 表明队列里面的payload都不持有生命周期，不会释放
	//       如果release != NULL, 则会对队列中的每一项调用release(payload)以释放资源
	void clear(void(*release)(void*) = NULL);

private:

	YCDList * myList;
};

#endif