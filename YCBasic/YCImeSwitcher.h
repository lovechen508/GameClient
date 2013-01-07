#ifndef _INCLUDE_YCIMESWITCHER_H_
#define _INCLUDE_YCIMESWITCHER_H_

#include "YCDef.h"

//
// YCImeSwitcher : ���뷨������
//
class YCAPI YCImeSwitcher
{
public:

    YCImeSwitcher(void);

    ~YCImeSwitcher(void);

    void bind2Window(HWND hWnd);

    //
    // ȡ�����뷨
    //
    void disable();

    //
    // �������뷨
    //
    void enable();

private:

    HWND myHWnd;

    HIMC myHImc;
    
    //
    // �ڴ�����С��֮�����myHImc�ᶪʧ
    //
    HIMC myLastValidImc;

};

#endif

