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
// 函数：with(unsigned int id, T*)
//
// 目的：设置内部必须数据，YCIFsmTransfer调用
//      不实现，子类实现
//
YCIFsmState* YCIFsmState::with(unsigned int id, void*)
{
	throw YCException(2002, "YCIFsmState::with(void*)未实现", __FUNCTION__);
}

YCIFsmState* YCIFsmState::with(unsigned int id, int)
{
	throw YCException(2002, "YCIFsmState::with(int)未实现", __FUNCTION__);
}

YCIFsmState* YCIFsmState::with(unsigned int id, unsigned int)
{
	throw YCException(2002, "YCIFsmState::with(int)未实现", __FUNCTION__);
}

YCIFsmState* YCIFsmState::with(unsigned int id, const char*)
{
	throw YCException(2002, "YCIFsmState::with(const char*)未实现", __FUNCTION__);
}
