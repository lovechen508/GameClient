#include "YCLuaContext.h"

#include <map>

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

#pragma region YCLuaContextImpl

/*
 * YCLuaContextImpl : 上下文内部实现类
 *                    主要原因是DLL不能暴露std容器
 */
class YCLuaContextImpl
{
public:

	YCLuaContextImpl()
		: mySelector(NULL)
	{
	}

	~YCLuaContextImpl()
	{
		if (mySelector != NULL)
		{
			mySelector->clear();
		}
		SAFE_DELETE(mySelector);
	}

	//
	// 函数：addContext(const char* key, void* context)
	//
	// 目的：设定一个上下文值
	//
	void addContext(const char* key, void* context)
	{
		std::map<std::string, void*>::iterator at = myContextMap.find(key);
		if (at != myContextMap.end())
		{
			throw YCException(2002, "YCLuaContext::addContext注册上下文值重复！");
		}
		myContextMap[key] = context;
	}

	//
	// 函数：removeContext(const char* key)
	//
	// 目的：设定一个上下文值
	//
	void* removeContext(const char* key)
	{
		void* result = NULL;
		std::map<std::string, void*>::iterator at = myContextMap.find(key);
		if (at == myContextMap.end())
		{
			result = at->second;
			myContextMap.erase(at);
		}
		return result;
	}

	//
	// 函数：updateContext(YCDList* selector);
	//
	// 目的：改变一个由YCQuery.selector遴选出的YCUITag节点列表
	//
	void updateSelector(YCDList* selector)
	{
		if (mySelector != NULL)
		{
			mySelector->clear();
		}
		SAFE_DELETE(mySelector);
		mySelector = selector;
	}

	//
	// 函数：lookup(const char* key)
	//
	// 目的：查询一个上下文值
	//
	void* lookup(const char* key)
	{
		SMART_ASSERT(key != NULL);

		void* found = NULL;
		if (strcmp("selector", key) == 0)
		{
			found = mySelector;
		}
		else
		{
			std::map<std::string, void*>::iterator at = myContextMap.find(key);
			if (at != myContextMap.end())
			{
				found = at->second;
			}
		}

		return found;
	}


private:

	// 容器内的context都不持有生命周期
	std::map<std::string, void*> myContextMap;

	// 由YCQuery.selector选出来的Tag列表，也不持有生命周期
	YCDList *mySelector;
};

#pragma endregion YCLuaContextImpl

YCLuaContext::YCLuaContext(void)
	: myContextImpl(new YCLuaContextImpl())
{
}


YCLuaContext::~YCLuaContext(void)
{
	SAFE_DELETE(myContextImpl);
}

//
// 函数：addContext(const char* key, void* context)
//
// 目的：设定一个上下文值
//
void YCLuaContext::addContext(const char* key, void* context)
{
	SMART_ASSERT(key != NULL);
	SMART_ASSERT(context != NULL);

	myContextImpl->addContext(key, context);
}

//
// 函数：updateContext(YCDList* selector);
//
// 目的：改变一个由YCQuery.selector遴选出的YCUITag节点列表
//
void YCLuaContext::updateSelector(YCDList* selector)
{	
	myContextImpl->updateSelector(selector);
}

//
// 函数：removeContext(const char* key);
//
// 目的：改变一个上下文值
//
void YCLuaContext::removeContext(const char* key)
{
	SMART_ASSERT(key != NULL);
	myContextImpl->removeContext(key);
}

//
// 函数：lookup(const char* key)
//
// 目的：查询一个上下文值
//
void* YCLuaContext::lookup(const char* key)
{
	SMART_ASSERT(key != NULL);
	return myContextImpl->lookup(key);
}
