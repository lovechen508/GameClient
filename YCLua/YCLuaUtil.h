#ifndef _INCLUDE_YCLUAUTIL_H_
#define _INCLUDE_YCLUAUTIL_H_

#include "YCBasic\YCDef.h"

/*
 * YCLuaUtil : C������¶��Lua�Ĺ��ߺ�����
 *             ������YCQuery�Ĵ�����
 */
struct YCLuaUtil
{
	//
	// ������alert_lua(const char *str, const char *title = NULL)
	//
	// Ŀ�ģ���lua�е���MessageBox
	//
	static int alert_lua(const char *str, const char *title = NULL);
};

#endif
