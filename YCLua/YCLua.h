#ifndef _INCLUDE_YCLUA_H_
#define _INCLUDE_YCLUA_H_

#include <stdarg.h>

struct lua_State;

class YCLuaState;
class YCLuaContext;
class YCIQueryFactory;

YCAPI int luaopen_ycquery(lua_State* ls);

/*
 * YCLua : Lua环境包装类
 */
class YCAPI YCLua 
{

public:
	
	YCLua();

	~YCLua();

	//
	// 函数：
	//
	// 目的：
	//

	//
	// 函数：bootstrap()
	//
	// 目的：引导Lua环境
	//
	bool bootstrap();

	//
	// 函数：bindQueryFactory(YCIQueryFactory* factory)
	//
	// 目的：绑定Factory工厂
	//
	void bindQueryFactory(YCIQueryFactory* factory);

	//
	// 函数：luaState()
	//
	// 目的：暴露内部lua_state
	//
	lua_State* luaState();

	//
	// 函数：execute(YCLuaContext* context, const char* luaCode, const char* blockName)
	//
	// 目的：在当前窗口\控件上下文中运行<code></code>中的Lua脚本
	//
	void execute(YCLuaContext* context, const char* luaCode, const char* blockName);

	//
	// 函数：call(const char *func, const char *sig, ...)
	//
	// 目的：通用Lua函数调用包装
	//
	void call(const char *func, const char *sig, ...);
	
private:
	
	YCLuaState * myLuaState;
};

#endif