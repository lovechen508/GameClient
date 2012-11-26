#include "YCRegistry.h"

#include <map>
#include <string>

#include "YCAssert.h"

/////////////////////////////////////////////////
// 全局变量管理器内部实现类
/////////////////////////////////////////////////

class YCRegistryImpl
{
	
	typedef std::map<std::string, void*> REGISTRYMAP;

public:
	
	YCRegistryImpl() 
	{
	}

	~YCRegistryImpl()
	{
		reset();
	}

	bool add(const char* key, void* val)
	{
		REGISTRYMAP::iterator at = myRegistry.find(key);
		if (at != myRegistry.end())
		{
			return false;
		}

		myRegistry[key] = val;
		return true;
	}

	void update(const char* key, void* val)
	{
		REGISTRYMAP::iterator at = myRegistry.find(key);
		if (at != myRegistry.end())
		{
			delete at->second;
		}

		myRegistry[key] = val;
	}

	void* get(const char* key)
	{
		REGISTRYMAP::iterator at = myRegistry.find(key);
		if (at != myRegistry.end())
		{
			return at->second;
		}
		return NULL;
	}

	void* remove(const char* key)
	{
		void * result = NULL;
		REGISTRYMAP::iterator at = myRegistry.find(key);
		if (at != myRegistry.end())
		{
			result = at->second;
			myRegistry.erase(at);
		}
		return result;
	}

	void reset()
	{
		/*
		for (REGISTRYMAP::iterator it = myRegistry.begin(); 
			 it != myRegistry.end(); ++it)
		{
			delete it->second;
		}
		*/

		myRegistry.clear();
	}

private:

	std::map<std::string, void*> myRegistry;
};

static YCRegistryImpl g_RegistryHolder;

void YCRegistry::initialize()
{
	//do nothing
}

bool YCRegistry::set(const char* key, void* val)
{
	SMART_ASSERT(key != NULL);
	SMART_ASSERT(val != NULL);

	if (key == NULL || val == NULL)
	{
		return false;
	}

	g_RegistryHolder.update(key, val);
	return true;
}

void* YCRegistry::get(const char* key)
{
	SMART_ASSERT(key != NULL);
	
	if (key == NULL)
	{
		return NULL;
	}

	return g_RegistryHolder.get(key);
}

void* YCRegistry::remove(const char* key)
{
	SMART_ASSERT(key != NULL);
	return g_RegistryHolder.remove(key);
}

void YCRegistry::finalize()
{
	g_RegistryHolder.reset();
}