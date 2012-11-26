#include "YCIFsmState.h"

#include "YCAssert.h"
#include "YCException.h"

YCIFsmState::YCIFsmState(YCIFsm* fsm)
	: myFsm(fsm)
{
	SMART_ASSERT(myFsm != NULL);
}

YCIFsmState::~YCIFsmState(void)
{
}

//
// ������with(unsigned int id, T*)
//
// Ŀ�ģ������ڲ��������ݣ�YCIFsmTransfer����
//      ��ʵ�֣�����ʵ��
//
YCIFsmState* YCIFsmState::with(unsigned int id, void*)
{
	throw YCException(2002, "YCIFsmState::with(void*)δʵ��", __FUNCTION__);
}

YCIFsmState* YCIFsmState::with(unsigned int id, int)
{
	throw YCException(2002, "YCIFsmState::with(int)δʵ��", __FUNCTION__);
}

YCIFsmState* YCIFsmState::with(unsigned int id, unsigned int)
{
	throw YCException(2002, "YCIFsmState::with(int)δʵ��", __FUNCTION__);
}

YCIFsmState* YCIFsmState::with(unsigned int id, const char*)
{
	throw YCException(2002, "YCIFsmState::with(const char*)δʵ��", __FUNCTION__);
}
