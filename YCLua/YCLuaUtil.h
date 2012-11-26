#ifndef _INCLUDE_YCLUAUTIL_H_
#define _INCLUDE_YCLUAUTIL_H_

#include "YCBasic\YCDef.h"

/*
 * YCLuaUtil : C环境暴露给Lua的工具函数组
 *             不属于YCQuery的串函数
 */
struct YCLuaUtil
{
	//
	// 函数：alert_lua(const char *str, const char *title = NULL)
	//
	// 目的：在lua中调用MessageBox
	//
	static int alert_lua(const char *str, const char *title = NULL);
};

#endif
