#include "YCLuaUtil.h"

//
// 函数：alert_lua(const char *str, const char *title = NULL)
//
// 目的：在lua中调用MessageBox
//
int YCLuaUtil::alert_lua(const char *str, const char *title)
{
  return MessageBox(HWND_DESKTOP, (LPSTR)str, (LPCSTR)title, MB_OK);
}

