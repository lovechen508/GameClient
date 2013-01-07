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
// ȡ�����뷨
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
// �������뷨
//
void YCImeSwitcher::enable()
{
    if (myHImc == NULL)
    {
        myHImc = myLastValidImc;
    }

    if (myHImc == NULL)
    {
        throw YCException(2002, "YCImeSwitcher::enable�޷��ָ����뷨");
    }

    ImmAssociateContext(myHWnd, myHImc);
}
