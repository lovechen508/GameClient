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
 * YCLuaParameterHolder : �ڲ�ʵ��ģʽ
 */
class YCLuaParameterHolder
{
public:

	YCLuaParameterHolder() {}

	~YCLuaParameterHolder() {}

	//
	// ������append(const char* params)
	//
	// Ŀ�ģ�����µ����ݵ�������
	//
	void append(const char* params)
	{
		myParameter += params;
		LOG_TRACE("YCLuaParameter ��ǰֵ��" << myParameter);
	}

	//
	// ������length() const
	//
	// Ŀ�ģ����ص�ǰ���ݳ���
	//
	int length() const
	{
		return myParameter.length();
	}

	//
	// ������toString()
	//
	// Ŀ�ģ����ش����ɹ����ַ���
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
// ������YCLuaParameter(lua_State* l, const char* name)
//
// Ŀ�ģ�����push��Lua�����еı�����
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
// ������enter(const char* name)
//
// Ŀ�ģ�����һ������
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
// ������item(const char* name, int value)
//
// Ŀ�ģ�����Intֵ
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
// ������item(const char* value)
//
// Ŀ�ģ������ַ���ֵ
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
// ������item(double value)
//
// Ŀ�ģ�����doubleֵ
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
// ������foreach(YCILuaForEachWrapper& wrapper)
//
// Ŀ�ģ������ӱ���
//
YCLuaParameter& YCLuaParameter::foreach(YCILuaForEachWrapper& wrapper)
{	
	if (!wrapper.handle(*this))
	{
		throw YCException(2002, "YCLuaParameter::foreach����ʧ�ܣ�");
	}
	return *this;
}

//
// ������leave()
//
// Ŀ�ģ��ص��ϼ�����
//
YCLuaParameter& YCLuaParameter::leave()
{	
	myParameters->append("}");
	myLastFlag = YALUAPARAMETER_LEAVE;
	return *this;
}

//
// ������push()
//
// Ŀ�ģ�����C++������Lua������ʹ��Lua�ű���
//       ����ͨ��YCStackǰ�����
//
void YCLuaParameter::push()
{
	std::string strParams = myName;
	strParams += "={";
	strParams += myParameters->toString();
	strParams += "}";

	LOG_TRACE("YCLuaParameter�����ɹ���" << strParams.c_str());

	// ע��YCLuaParameter
	if (luaL_loadbuffer(L, strParams.c_str(), strParams.size(), myName) || lua_pcall(L, 0, 0, 0)) 
	{
		LOG_FATAL("YCLuaParameter::pushע��ȫ�ֱ���ʧ��:" << lua_tostring(L, -1));
		lua_pop(L, 1);
		throw YCException(2002, "YCLuaParameter::pushע��ȫ�ֱ���ʧ��!");
	}
}

//
// ������dump()
//
// Ŀ�ģ�dumpջ�ڲ�״̬����־�ļ�
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
// ������checkStack(int sz)
//
// Ŀ�ģ�ȷ��ջ����ס���Ŀռ䱣��c��Lua���ݵ�����
//
bool YCLuaParameter::checkStack(int sz)
{
	return true;
}