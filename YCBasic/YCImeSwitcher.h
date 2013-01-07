#ifndef _INCLUDE_YCIMESWITCHER_H_
#define _INCLUDE_YCIMESWITCHER_H_

#include "YCDef.h"

//
// YCImeSwitcher : 输入法开关类
//
class YCAPI YCImeSwitcher
{
public:

    YCImeSwitcher(void);

    ~YCImeSwitcher(void);

    void bind2Window(HWND hWnd);

    //
    // 取消输入法
    //
    void disable();

    //
    // 激活输入法
    //
    void enable();

private:

    HWND myHWnd;

    HIMC myHImc;
    
    //
    // 在窗口最小化之后可能myHImc会丢失
    //
    HIMC myLastValidImc;

};

#endif

