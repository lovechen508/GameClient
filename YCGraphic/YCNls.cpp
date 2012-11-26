#include "YCNls.h"

#include "YCBasic\YCAssert.h"

//
// 函数：
//
// 目的：
//
YCNls::YCNls(const char* filename)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(strlen(filename) < MAX_PATH);

	memset(myNlsFile, 0, MAX_PATH);
	memcpy(myNlsFile, filename, strlen(filename));
}

//
// 函数：
//
// 目的：
//
YCNls::~YCNls(void)
{

}

//
// 函数：initialize()
//
// 目的：读取NLS配置文件
//
bool YCNls::initialize()
{
	return true;
}

//
// 函数：lookup(const char* key)
//
// 目的：查找NLS名称
//
const char* YCNls::lookup(const char* key)
{
	return NULL;
}
