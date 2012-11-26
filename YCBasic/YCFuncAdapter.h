#ifndef _INCLUDE_YCFUNCADAPTER_H_
#define _INCLUDE_YCFUNCADAPTER_H_

#include <tuple>

#include "YCAssert.h"

/*
 * YCIFuncAdapter : 函数适配器
 * 
 * 说明：适用于从一个对象携带任意参数
 *      转发消息到任意对象
 */
template<typename T>
class YCFuncAdapter
{
public:

	

public:

	//
	// T 为接收消息的对象
	//
	YCFuncAdapter(T* t)
		: myDelegate(t)
	{
		SMART_ASSERT(t != NULL);
	}

	~YCFuncAdapter() {}

	//
	// 函数：forward(P tupleInfo)
	//
	// 目的：转发函数
	//
	template<typename P>
	void forward(P& tupleInfo)
	{
		myDelegate->handle(tupleInfo);
	}

	template<typename P1, typename P2>
	void forward(P1 p1, P2 p2)
	{
		myDelegate->handle(p1, p2);
	}

	template<typename P1, typename P2, typename P3>
	void forward(P1 p1, P2 p2, P3 p3)
	{
		myDelegate->handle(p1, p2, p3);
	}

private:

	T* myDelegate;
};


#endif

