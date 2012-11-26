#ifndef _INCLUDE_YCSTACK_H_
#define _INCLUDE_YCSTACK_H_

class YCDList;

class YCAPI YCStack
{
public:

	YCStack(void);

	virtual ~YCStack(void);

	//
	// 函数：push(void * payload)
	//
	// 目的：压栈
	//
	void push(void * payload);

	//
	// 函数：front()
	//
	// 目的：返回栈定元素，不弹出
	//
	void* front();

	//
	// 函数：pop()
	//
	// 目的：弹出并返回栈顶元素
	//
	void* pop();
	
private:

	YCDList * myList;
};

#endif