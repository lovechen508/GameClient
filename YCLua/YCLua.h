#ifndef _INCLUDE_YCLUA_H_
#define _INCLUDE_YCLUA_H_

#include <stdarg.h>

struct lua_State;

class YCLuaState;
class YCLuaContext;
class YCIQueryFactory;

YCAPI int luaopen_ycquery(lua_State* ls);

/*
 * YCLua : Lua������װ��
 */
class YCAPI YCLua 
{

public:
	
	YCLua();

	~YCLua();

	//
	// ������
	//
	// Ŀ�ģ�
	//

	//
	// ������bootstrap()
	//
	// Ŀ�ģ�����Lua����
	//
	bool bootstrap();

	//
	// ������bindQueryFactory(YCIQueryFactory* factory)
	//
	// Ŀ�ģ���Factory����
	//
	void bindQueryFactory(YCIQueryFactory* factory);

	//
	// ������luaState()
	//
	// Ŀ�ģ���¶�ڲ�lua_state
	//
	lua_State* luaState();

	//
	// ������execute(YCLuaContext* context, const char* luaCode, const char* blockName)
	//
	// Ŀ�ģ��ڵ�ǰ����\�ؼ�������������<code></code>�е�Lua�ű�
	//
	void execute(YCLuaContext* context, const char* luaCode, const char* blockName);

	//
	// ������call(const char *func, const char *sig, ...)
	//
	// Ŀ�ģ�ͨ��Lua�������ð�װ
	//
	void call(const char *func, const char *sig, ...);
	
private:
	
	YCLuaState * myLuaState;
};

#endif