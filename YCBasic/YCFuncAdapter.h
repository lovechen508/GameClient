#ifndef _INCLUDE_YCFUNCADAPTER_H_
#define _INCLUDE_YCFUNCADAPTER_H_

#include <tuple>

#include "YCAssert.h"

/*
 * YCIFuncAdapter : ����������
 * 
 * ˵���������ڴ�һ������Я���������
 *      ת����Ϣ���������
 */
template<typename T>
class YCFuncAdapter
{
public:

	

public:

	//
	// T Ϊ������Ϣ�Ķ���
	//
	YCFuncAdapter(T* t)
		: myDelegate(t)
	{
		SMART_ASSERT(t != NULL);
	}

	~YCFuncAdapter() {}

	//
	// ������forward(P tupleInfo)
	//
	// Ŀ�ģ�ת������
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

