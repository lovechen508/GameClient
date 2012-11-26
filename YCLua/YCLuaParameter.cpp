#include "YCLuaParameter.h"

#include <string>
#include <sstream>

#include "lua.hpp"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCStack.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

#define YALUAPARAMETER_LEAVE ','
#define YCLUAPARAMETER_EQUAL '='
#define YCLUAPARAMETER_BRACE '{'
#define YCLUAPARAMETER_ECARB '{'

#pragma region YCLuaParameterHolder

/*
 * YCLuaParameterHolder : 内部实现模式
 */
class YCLuaParameterHolder
{
public:

	YCLuaParameterHolder() {}

	~YCLuaParameterHolder() {}

	//
	// 函数：append(const char* params)
	//
	// 目的：添加新的数据到上下文
	//
	void append(const char* params)
	{
		myParameter += params;
		LOG_TRACE("YCLuaParameter 当前值：" << myParameter);
	}

	//
	// 函数：length() const
	//
	// 目的：返回当前数据长度
	//
	int length() const
	{
		return myParameter.length();
	}

	//
	// 函数：toString()
	//
	// 目的：返回创建成功的字符串
	//
	const std::string& toString() const
	{
		return myParameter;
	}

private:

	std::string myParameter;
};

#pragma endregion YCLuaParameterHolder

//
// 函数：YCLuaParameter(lua_State* l, const char* name)
//
// 目的：设置push到Lua环境中的变量名
//
YCLuaParameter::YCLuaParameter(lua_State* l, const char* name)
	: L(l)
	, myLastFlag(0)
	, myParameters(new YCLuaParameterHolder())
{
	SMART_ASSERT(l != NULL);
	SMART_ASSERT(name != NULL);
	SMART_ASSERT(strlen(name) < 16);

	memset(myName, 0, 16);
	memcpy(myName, name, strlen(name));
}

YCLuaParameter::~YCLuaParameter()
{
	SAFE_DELETE(myParameters);
}

//
// 函数：enter(const char* name)
//
// 目的：创建一个表项
//
YCLuaParameter& YCLuaParameter::enter(const char* name)
{
	if (myLastFlag == YALUAPARAMETER_LEAVE)
	{
		myParameters->append(",");
	}

	if (name != NULL)
	{
		myParameters->append(name);
		myParameters->append("=");
	}
	myParameters->append("{");
	myLastFlag = 0;
	return *this;
}

//
// 函数：item(const char* name, int value)
//
// 目的：设置Int值
//
YCLuaParameter& YCLuaParameter::item(const char* name, int value)
{
	if (myLastFlag == YALUAPARAMETER_LEAVE)
	{
		myParameters->append(",");
	}

	std::stringstream ss;
	ss << name << "=" << value;
	myParameters->append(ss.str().c_str());
	myLastFlag = YALUAPARAMETER_LEAVE;
	return *this;
}

//
// 函数：item(const char* value)
//
// 目的：设置字符串值
//
YCLuaParameter& YCLuaParameter::item(const char* name, const char * value)
{
	if (myLastFlag == YALUAPARAMETER_LEAVE)
	{
		myParameters->append(",");
	}

	myParameters->append(name);
	myParameters->append("=");
	myParameters->append("\"");
	myParameters->append(value);
	myParameters->append("\"");
	myLastFlag = YALUAPARAMETER_LEAVE;
	return *this;
}

//
// 函数：item(double value)
//
// 目的：设置double值
//
YCLuaParameter& YCLuaParameter::item(const char* name, double value)
{	
	if (myLastFlag == YALUAPARAMETER_LEAVE)
	{
		myParameters->append(",");
	}

	std::stringstream ss;
	ss << name << "=" << value;
	myParameters->append(ss.str().c_str());
	myLastFlag = YALUAPARAMETER_LEAVE;
	return *this;
}

//
// 函数：foreach(YCILuaForEachWrapper& wrapper)
//
// 目的：设置子表项
//
YCLuaParameter& YCLuaParameter::foreach(YCILuaForEachWrapper& wrapper)
{	
	if (!wrapper.handle(*this))
	{
		throw YCException(2002, "YCLuaParameter::foreach处理失败：");
	}
	return *this;
}

//
// 函数：leave()
//
// 目的：回到上级表项
//
YCLuaParameter& YCLuaParameter::leave()
{	
	myParameters->append("}");
	myLastFlag = YALUAPARAMETER_LEAVE;
	return *this;
}

//
// 函数：push()
//
// 目的：推送C++变量到Lua环境，使在Lua脚本中
//       可以通过YCStack前最访问
//
void YCLuaParameter::push()
{
	std::string strParams = myName;
	strParams += "={";
	strParams += myParameters->toString();
	strParams += "}";

	LOG_TRACE("YCLuaParameter构建成功：" << strParams.c_str());

	// 注册YCLuaParameter
	if (luaL_loadbuffer(L, strParams.c_str(), strParams.size(), myName) || lua_pcall(L, 0, 0, 0)) 
	{
		LOG_FATAL("YCLuaParameter::push注册全局变量失败:" << lua_tostring(L, -1));
		lua_pop(L, 1);
		throw YCException(2002, "YCLuaParameter::push注册全局变量失败!");
	}
}

//
// 函数：dump()
//
// 目的：dump栈内部状态到日志文件
//
void YCLuaParameter::dump()
{
	LOG_DEBUG("Lua Stack : ");
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {
		int t = lua_type(L, i);
		switch (t) {
			case LUA_TSTRING: /* strings */
				LOG_DEBUG("	" << i << ": " << lua_tostring(L, i));
				break;
			case LUA_TBOOLEAN: /* booleans */
				LOG_DEBUG("	" << i << ": " << (lua_toboolean(L, i) ? "true" : "false"));
				break;
			case LUA_TNUMBER: /* numbers */
				LOG_DEBUG("	" << i << ": " << lua_tonumber(L, i));
				break;
			default: /* other values */
				LOG_DEBUG("	" << i << ": " << lua_typename(L, t));
				break;
		}
		LOG_DEBUG(" "); 
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Private Part
////////////////////////////////////////////////////////////////////////////////////
//
// 函数：checkStack(int sz)
//
// 目的：确保栈上有住够的空间保存c到Lua传递的数据
//
bool YCLuaParameter::checkStack(int sz)
{
	return true;
}