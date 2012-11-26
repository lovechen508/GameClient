#include "YCNls.h"

#include "YCBasic\YCAssert.h"

//
// ������
//
// Ŀ�ģ�
//
YCNls::YCNls(const char* filename)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(strlen(filename) < MAX_PATH);

	memset(myNlsFile, 0, MAX_PATH);
	memcpy(myNlsFile, filename, strlen(filename));
}

//
// ������
//
// Ŀ�ģ�
//
YCNls::~YCNls(void)
{

}

//
// ������initialize()
//
// Ŀ�ģ���ȡNLS�����ļ�
//
bool YCNls::initialize()
{
	return true;
}

//
// ������lookup(const char* key)
//
// Ŀ�ģ�����NLS����
//
const char* YCNls::lookup(const char* key)
{
	return NULL;
}
