#ifndef _INCLUDE_YCSINGLETON_H_
#define _INCLUDE_YCSINGLETON_H_

#include "YCAssert.h"

/*
 *  YCSingleton<T> : 自动Singleton基类
 */
template<typename T>
class YCSingleton
{
	static T* gSingleton;

public:

	static T& GetSingleton(void)
	{
		SMART_ASSERT(gSingleton);
		return (*gSingleton);
	}
	
	static T* GetSingletonPtr(void)
	{
		return gSingleton;
	}

protected:

	YCSingleton(void)
	{
		SMART_ASSERT(!gSingleton);
		int offset = (int)(T*)1 - (int)(YCSingleton<T>*)(T*)1;
		gSingleton = (T*)((int)this + offset);
	}

	~YCSingleton(void)
	{
		SMART_ASSERT(gSingleton);
		gSingleton = NULL;
	}
};

template<typename T> T* YCSingleton<T>::gSingleton = NULL;

#endif