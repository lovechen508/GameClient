#include "YCImeSwitcher.h"

#include <Imm.h>

#include "YCAssert.h"
#include "YCException.h"

YCImeSwitcher::YCImeSwitcher(void)
{
}


YCImeSwitcher::~YCImeSwitcher(void)
{
}

void YCImeSwitcher::bind2Window(HWND hWnd)
{
    SMART_ASSERT(hWnd != NULL);
    myHWnd = hWnd;
}

//
// 取消输入法
//
void YCImeSwitcher::disable()
{
    myHImc = ::ImmAssociateContext(myHWnd, 0);
    if (myHImc != 0)
    {
        myLastValidImc = myHImc;
    }
}

//
// 激活输入法
//
void YCImeSwitcher::enable()
{
    if (myHImc == NULL)
    {
        myHImc = myLastValidImc;
    }

    if (myHImc == NULL)
    {
        throw YCException(2002, "YCImeSwitcher::enable无法恢复输入法");
    }

    ImmAssociateContext(myHWnd, myHImc);
}
