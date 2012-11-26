#include "YCLua.h"

#include "lua.hpp"

#include "YCIQuery.h"
#include "YCLuaUtil.h"
#include "YCLuaParameter.h"
#include "YCIQueryFactory.h"

#include "YCBasic\YCStack.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCSingleton.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCScopeGuard.h"

class YCLuaState;

#pragma region YCLuaState

#define LUA_PREPARE_CONTEXT YCLuaContext* context = (YCLuaContext*)YCLuaState::GetSingleton().myLuaStack->front(); \
	                        if (context == NULL) \
							{ \
								throw YCException(2002, "YCLua��������û���ҵ���Ӧ��YCLuaContext�����ģ�"); \
							}

#define LUA_CHAIN_RETURN(code) lua_getglobal(L, "YCQuery");\
                               return (code);

class YCLuaState
	: public YCSingleton<YCLuaState>
{
public:

	#pragma region YCQuery

	/* push the number back to satck but increased by 1 */
	//lua_pushnumber(L, i+2);
	/* return the number of results */	

	static int $(lua_State* L)
	{
		const char* sel = luaL_checkstring(L, -1);
		LOG_DEBUG("YCLuaState::$ called : " << sel);

		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);
		query->$(sel);

		LUA_CHAIN_RETURN(1);
	}

	static int css(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int attr(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int html(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		const char* html = luaL_checkstring(L, -1);
		LOG_DEBUG("YCLuaState::html called : " << html);

		YCIQuery* query = gQueryFactory->createYCQuery(context);
		query->html(html);

		LUA_CHAIN_RETURN(1);
	}

	static int value(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;
		
		YCIQuery* query = gQueryFactory->createYCQuery(context);
		
		if (lua_gettop(L) == 1)
		{
			const char* value = luaL_checkstring(L, -1);
			LOG_DEBUG("YCLuaState::value called : " << value);
			query->value(value);

			LUA_CHAIN_RETURN(1);
		}
		
		const char* value = query->value();
		lua_pushstring(L, value);
		return 1;
	}

	static int append(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		const char* html = luaL_checkstring(L, -1);
		LOG_DEBUG("YCLuaState::append called : " << html);

		YCIQuery* query = gQueryFactory->createYCQuery(context);
		query->append(html);

		LUA_CHAIN_RETURN(1);
	}

	static int each(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int hover(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int toggle(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int open(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int close(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int show(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int hide(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int fadeIn(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int fadeOut(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int animate(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int move(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int parent(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int child(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int click(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int submit(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int slideUp(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int slideDown(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int addClass(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int removeClass(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}

	static int end(lua_State* L)
	{
		LUA_PREPARE_CONTEXT;

		YCIQuery* query = gQueryFactory->createYCQuery(context);

		LUA_CHAIN_RETURN(1);
	}
	
	///////////////////////////////////////////
	// ��Chain����
	///////////////////////////////////////////
	static int alert(lua_State* L)
	{
		int n = lua_gettop(L);
		if(n < 1)
		{
			lua_pushstring(L, "alert_glue: not enough arguments");
			lua_error(L);
		}

		const char *str = lua_tostring(L, 1);
		if(str == NULL)
		{
			lua_pushstring(L, "alert_glue: non-string");
			lua_error(L);
		}

		if(n == 1)
		{
			lua_pushnumber(L, YCLuaUtil::alert_lua(str));
		}
		else
		{
			lua_pushnumber(L, YCLuaUtil::alert_lua(str, lua_tostring(L, 2)));
		}

		return 1;
	}
	
#pragma endregion YCQuery

public:

	YCLuaState() 
		: L(NULL) 
		, myLuaStack(new YCStack())
	{
	}

	~YCLuaState() 
	{
		SAFE_DELETE(myLuaStack);
	}

	//
	// ������bootstrap()
	//
	// Ŀ�ģ�����Lua����
	//
	bool bootstrap()
	{
		L = lua_open(); /* opens Lua */
		if (L == NULL)
		{
			return false;
		}

		luaL_openlibs(L); /* Opens all standard Lua libraries into the given state. */

		luaopen_ycquery(L);

		// ���YCQuery�Ƿ��Ѿ�ע��
		lua_getglobal(L, "YCQuery");
		if (!lua_isnil(L, -1))
		{
			LOG_FATAL("YCLuaState::bootstrap��YCQuery�Ѿ�ע�ᣡ");
			return false;
		}
		else 
		{
			lua_pop(L, -1);
		}

		const char* chainTable = "YCQuery = { \
			selector=YCLua.selector, \
			css=YCLua.css, \
			value=YCLua.value, \
			html=YCLua.html, \
			append=YCLua.append, \
			each=YCLua.each, \
			hover=YCLua.hover, \
			toggle=YCLua.toggle, \
			open=YCLua.open, \
			close=YCLua.close, \
			show=YCLua.show, \
			hide=YCLua.hide, \
			fadeIn=YCLua.fadeIn, \
			fadeOut=YCLua.fadeOut, \
			animate=YCLua.animate, \
			move=YCLua.move, \
			parent=YCLua.parent, \
			child=YCLua.child, \
			click=YCLua.click, \
			submit=YCLua.submit, \
			slideUp=YCLua.slideUp, \
			slideDown=YCLua.slideDown, \
			addClass=YCLua.addClass, \
			removeClass=YCLua.removeClass, \
			finish=YCLua.finish, \
		}";

		// ע�ắ��������
		if (luaL_loadbuffer(L, chainTable, strlen(chainTable), "chain") || lua_pcall(L, 0, 0, 0)) 
		{
			LOG_FATAL("YCLuaState::bootstrapע�ắ��������ʧ��:" << lua_tostring(L, -1));
			lua_pop(L, 1);
		}

		//if (luaL_loadfile(L, "c:\\lua\\avatar.lua") || lua_pcall(L, 0, 0, 0))
		//{
		//	LOG_FATAL("cannot run configuration file: " << lua_tostring(L, -1));
		//	lua_pop(L, 1);
		//}

		return true;
	}

	//
	// ������bindQueryFactory(YCIQueryFactory* factory)
	//
	// Ŀ�ģ���Factory����
	//
	void bindQueryFactory(YCIQueryFactory* factory)
	{
		SMART_ASSERT(factory != NULL);
		gQueryFactory = factory;
	}

	//
	// ������execute(YCLuaContext* context, const char* luaCode, const char* blockName)
	//
	// Ŀ�ģ��ڵ�ǰ����\�ؼ�������������<code></code>�е�Lua�ű�
	//
	void execute(YCLuaContext* context, const char* luaCode, const char* blockName)
	{		
		try
		{			
			// ѹ��ǰ�����Ľ�ջ
			myLuaStack->push(context);

			// ȷ����ջ
			//YCScopeGuard<YCStack, void*> POP_GUARD(myLuaStack, &YCStack::pop);
			SCOPE_GUARD(stack, myLuaStack->pop());
			
			if (luaL_loadbuffer(L, luaCode, strlen(luaCode), blockName) || lua_pcall(L, 0, 0, 0)) 
			{
				LOG_ERROR("YCLua::execute Lua�ű�["<< blockName << "]ִ���쳣:" << lua_tostring(L, -1));
				lua_pop(L, 1);
				throw YCException(2002, "YCLua::execute Lua�ű�ִ��δ֪�쳣");
			}
		}
		catch(YCException& e)
		{
			LOG_ERROR("YCLua::execute Lua�ű�ִ���쳣��" << e.what());
			throw;
		}
		catch(...)
		{
			LOG_ERROR("YCLua::execute Lua�ű�ִ���쳣��" << luaCode);
			throw YCException(2002, "YCLua::execute Lua�ű�ִ��δ֪�쳣: ");
		}
	}

	//
	// ������call(const char *func, const char *sig, ...)
	//
	// Ŀ�ģ�ͨ��Lua�������ð�װ
	//
	void call(const char *func, const char *sig, ...)
	{
		va_list vl;
		int narg, nres; /* number of arguments and results */
		va_start(vl, sig);
		lua_getglobal(L, func); /* get function */
		/* push arguments */
		narg = 0;
		while (*sig) { /* push arguments */
			switch (*sig++) {
				case 'd': /* double argument */
					lua_pushnumber(L, va_arg(vl, double));
					break;
				case 'i': /* int argument */
					lua_pushnumber(L, va_arg(vl, int));
					break;
				case 's': /* string argument */
					lua_pushstring(L, va_arg(vl, char *));
					break;
				case '>':
					goto endwhile;
				default:
					LOG_FATAL("YCLua::call invalid option :" << *(sig - 1));
			}
			narg++;
			luaL_checkstack(L, 1, "too many arguments");
		} 

		endwhile:
		/* do the call */
		nres = strlen(sig); /* number of expected results */
		if (lua_pcall(L, narg, nres, 0) != 0)
		{
			LOG_FATAL("YCLua::call error running function " << func << ": " << lua_tostring(L, -1));
		}
		
		/* retrieve results */
		nres = -nres; /* stack index of first result */
		while (*sig) { /* get results */
			switch (*sig++) {
				case 'd': /* double result */
					if (!lua_isnumber(L, nres))
					LOG_FATAL("YCLua::call wrong result type");
					*va_arg(vl, double *) = lua_tonumber(L, nres);
					break;
				case 'i': /* int result */
					if (!lua_isnumber(L, nres))
					LOG_FATAL("YCLua::call wrong result type");
					*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
					break;
				case 's': /* string result */
					if (!lua_isstring(L, nres))
					LOG_FATAL("YCLua::call wrong result type");
					*va_arg(vl, const char **) = lua_tostring(L, nres);
					break;
				default:
					LOG_FATAL("YCLua::call invalid option :" << *(sig - 1));
			}
			nres++;
		}
		va_end(vl);
	}

	//
	// ������close()
	//
	// Ŀ�ģ�ж��Lua����
	//
	void close()
	{
		lua_close(L);
	}

	//
	// ������getLuaState()
	//
	// Ŀ�ģ��Զ�ת������
	//
	lua_State* getLuaState()
	{
		if (L == NULL)
		{
			throw YCException(2002, "YCLuaState::lua_State��Lua������δ��ʼ��!");
		}
		return L;
	}

private:

	lua_State * L;

	YCStack * myLuaStack;

	static YCIQueryFactory* gQueryFactory;
};

YCIQueryFactory* YCLuaState::gQueryFactory = NULL;

#pragma endregion YCLuaState

YCLua::YCLua()
	: myLuaState(new YCLuaState())
{
}

YCLua::~YCLua()
{
	if (myLuaState != NULL)
	{
		myLuaState->close();
		delete myLuaState;
	}
	myLuaState = NULL;
}

//
// ������bootstrap()
//
// Ŀ�ģ�����Lua����
//
bool YCLua::bootstrap()
{
	if (!myLuaState->bootstrap())
	{
		throw YCException(2002, "YCLua::Instance��ʼ��lua����ʧ��!");
	}
	return true;
}

//
// ������bindQueryFactory(YCIQueryFactory* factory)
//
// Ŀ�ģ���Factory����
//
void YCLua::bindQueryFactory(YCIQueryFactory* factory)
{
	myLuaState->bindQueryFactory(factory);
}

//
// ������luaState()
//
// Ŀ�ģ���¶�ڲ�lua_state
//
lua_State* YCLua::luaState()
{
	return myLuaState->getLuaState();
}

//
// ������execute(YCLuaContext* context, const char* luaCode, const char* blockName)
//
// Ŀ�ģ��ڵ�ǰ����\�ؼ�������������<code></code>�е�Lua�ű�
//
void YCLua::execute(YCLuaContext* context, const char* luaCode, const char* blockName)
{
	SMART_ASSERT(context != NULL);
	SMART_ASSERT(luaCode != NULL);

	myLuaState->execute(context, luaCode, blockName);
}

//
// ������call(const char *func, const char *sig, ...)
//
// Ŀ�ģ�ͨ��Lua�������ð�װ
//
void YCLua::call(const char *func, const char *sig, ...)
{
	va_list argp;
	va_start(argp, sig);
	myLuaState->call(func, argp);
	va_end(argp);
}

#pragma region YCLua

// ע��YCQuery����
static const struct luaL_reg ycquery[] = {
	{"selector",	YCLuaState::$},
	{"css",			YCLuaState::css},
	{"value",       YCLuaState::value},
	{"html",		YCLuaState::html},
	{"append",		YCLuaState::append},
	{"each",		YCLuaState::each},
	{"hover",		YCLuaState::hover},
	{"toggle",		YCLuaState::toggle},
	{"open",		YCLuaState::open},
	{"close",		YCLuaState::close},
	{"show",		YCLuaState::show},
	{"hide",		YCLuaState::hide},
	{"fadeIn",		YCLuaState::fadeIn},
	{"fadeOut",		YCLuaState::fadeOut},
	{"animate",		YCLuaState::animate},
	{"move",		YCLuaState::move},
	{"parent",		YCLuaState::parent},
	{"child",		YCLuaState::child},
	{"click",		YCLuaState::click},
	{"submit",		YCLuaState::submit},
	{"slideUp",		YCLuaState::slideUp},
	{"slideDown",	YCLuaState::slideDown},
	{"addClass",    YCLuaState::addClass},
	{"removeClass", YCLuaState::removeClass},
	{"finish",      YCLuaState::end},
	{"alert",       YCLuaState::alert},
	{NULL, NULL} /* sentinel */
};

//
// ������luaopen_ycquery(lua_State* L)
//
// Ŀ�ģ���Luaע��c������YCQueryImpl�޶�������
//
int luaopen_ycquery(lua_State* L)
{
	luaL_register(L, "YCLua", ycquery);
	return 1;
}

#pragma endregion YCLua