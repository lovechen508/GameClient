#include "YCLuaUtil.h"

//
// ������alert_lua(const char *str, const char *title = NULL)
//
// Ŀ�ģ���lua�е���MessageBox
//
int YCLuaUtil::alert_lua(const char *str, const char *title)
{
  return MessageBox(HWND_DESKTOP, (LPSTR)str, (LPCSTR)title, MB_OK);
}

