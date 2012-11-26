#include "YCLuaContext.h"

#include <map>

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

#pragma region YCLuaContextImpl

/*
 * YCLuaContextImpl : �������ڲ�ʵ����
 *                    ��Ҫԭ����DLL���ܱ�¶std����
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
	// ������addContext(const char* key, void* context)
	//
	// Ŀ�ģ��趨һ��������ֵ
	//
	void addContext(const char* key, void* context)
	{
		std::map<std::string, void*>::iterator at = myContextMap.find(key);
		if (at != myContextMap.end())
		{
			throw YCException(2002, "YCLuaContext::addContextע��������ֵ�ظ���");
		}
		myContextMap[key] = context;
	}

	//
	// ������removeContext(const char* key)
	//
	// Ŀ�ģ��趨һ��������ֵ
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
	// ������updateContext(YCDList* selector);
	//
	// Ŀ�ģ��ı�һ����YCQuery.selector��ѡ����YCUITag�ڵ��б�
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
	// ������lookup(const char* key)
	//
	// Ŀ�ģ���ѯһ��������ֵ
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

	// �����ڵ�context����������������
	std::map<std::string, void*> myContextMap;

	// ��YCQuery.selectorѡ������Tag�б�Ҳ��������������
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
// ������addContext(const char* key, void* context)
//
// Ŀ�ģ��趨һ��������ֵ
//
void YCLuaContext::addContext(const char* key, void* context)
{
	SMART_ASSERT(key != NULL);
	SMART_ASSERT(context != NULL);

	myContextImpl->addContext(key, context);
}

//
// ������updateContext(YCDList* selector);
//
// Ŀ�ģ��ı�һ����YCQuery.selector��ѡ����YCUITag�ڵ��б�
//
void YCLuaContext::updateSelector(YCDList* selector)
{	
	myContextImpl->updateSelector(selector);
}

//
// ������removeContext(const char* key);
//
// Ŀ�ģ��ı�һ��������ֵ
//
void YCLuaContext::removeContext(const char* key)
{
	SMART_ASSERT(key != NULL);
	myContextImpl->removeContext(key);
}

//
// ������lookup(const char* key)
//
// Ŀ�ģ���ѯһ��������ֵ
//
void* YCLuaContext::lookup(const char* key)
{
	SMART_ASSERT(key != NULL);
	return myContextImpl->lookup(key);
}
