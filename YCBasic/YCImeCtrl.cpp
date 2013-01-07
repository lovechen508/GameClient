#include "YCImeCtrl.h"

#include "YCDef.h"
#include "YCAssert.h"

YCImeCtrl::YCImeCtrl(void)
    : myILHead(NULL)
{
}


YCImeCtrl::~YCImeCtrl(void)
{
    SMART_ASSERT(myILHead != NULL);
}

//
// 初始化，从注册表中读取信息
//
bool YCImeCtrl::initialize()
{
    /*
    HKEY hKey,hKey1;
    DWORD cp=16;
    char lp[15];

    CString szID;
    const char* szKeyName = "Keyboard Layout\\Preload";
    const char* szKeyName1 = "System\\CurrentControlSet\\Control\\Keyboard Layouts\\";
    
    int i=1;      　
    szID.Format("%d",i);

    DWORD lpT=REG_SZ;
    if(::RegOpenKey(HKEY_CURRENT_USER,szKeyName,&hKey)==ERROR_SUCCESS )
    {
        while( ::RegQueryValueEx(hKey,szID,NULL,&lpT,(LPBYTE)lp,&cp) == ERROR_SUCCESS )
        {
            CString szTempName;
            szTempName = szKeyName1 + (LPCTSTR)(LPTSTR)lp;
            if(RegOpenKey(HKEY_LOCAL_MACHINE,szTempName,&hKey1)==ERROR_SUCCESS )
            {
                char lpD[100];
                DWORD lpS=100; //DataSize
                if(RegQueryValueEx(hKey1,"Layout text",NULL,&lpT,(LPBYTE)lpD,&lpS)==ERROR_SUCCESS)
                {
                    IL* p1,*p2;
                    p1 = myILHead;
                    p2 = new(IL);
                    strcpy(p2->ilID,lp);
                    strcpy(p2->szName,lpD);
                    p2->pNext = NULL;
                    if (p1)
                    {
                        while( p1->pNext )
                        {
                            p1 = p1->pNext ; 
                        }
                        p1->pNext = p2;
                    }
                    else
                    {
                        myILHead = p2;
                    }
                }
            }
                          　
            ::RegCloseKey(hKey1);
            i++;
            szID.Format("%d",i);
        }
    }
    
    ::RegCloseKey(hKey);
    */
    return (myILHead != NULL);
}

//
//  取得输入法列表
//
const YCImeCtrl::IL* YCImeCtrl::getHeader()
{
    return myILHead;
}

//
// 选择输入法
//
bool YCImeCtrl::selectIme(const IL* pIL)
{
    if( !pIL ) return FALSE;

    HKL hkl = LoadKeyboardLayout(pIL->ilID,KLF_ACTIVATE);//装载输入法
    if(hkl == NULL) 
    {
        return FALSE;
    }
    else
    {
        ActivateKeyboardLayout(hkl,KLF_SETFORPROCESS);//激活输入法
    }

    return TRUE;
}

//
// 释放已分配资源
//
void YCImeCtrl::finalize()
{
    IL* p1,*p2;
    p1 = myILHead;

    while (p1)
    {
        p2 = p1;
        p1 = p1->pNext;
        delete(p2);
    }
    
    myILHead = NULL;
}