#ifndef _INCLUDE_YCREGISTRY_H_
#define _INCLUDE_YCREGISTRY_H_

/*
 * YCRegistry : ȫ�ֱ���������
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
