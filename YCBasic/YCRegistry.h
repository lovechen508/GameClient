#ifndef _INCLUDE_YCREGISTRY_H_
#define _INCLUDE_YCREGISTRY_H_

/*
 * YCRegistry : 全局变量管理器
 */
class YCAPI YCRegistry
{
public:

	static void initialize();

	static bool set(const char* key, void* val);

	static void* get(const char* key);

	static void* remove(const char* key);

	static void finalize();

private:

	YCRegistry(void) {}

	~YCRegistry(void) {}
};

#endif
