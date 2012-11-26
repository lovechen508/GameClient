#ifndef _INCLUDE_YCLUASTACK_H_
#define _INCLUDE_YCLUASTACK_H_

#include "YCBasic\YCAssert.h"

struct lua_State;

class YCStack;
class YCLuaParameterHolder;

/*
 * YCLuaParameter : 
 *              游戏中呼出UI界面时传递值到Lua环境
 *              DLS 流畅API设计思路
 *
 * 说明：
 *
 *     YCParameter={
 *	 	    			x=0,                                       // value("x", 0)
 *						y=0,                                       // value("y", 0)
 *						z={                                        // foreach("z", YCILuaForEachWrapper)
 *							{surname="liao", lastname="zanxu"},
 *                          {surname="liao", lastname="yuchen"},
 *							l={surname="chen", lastname="li"}
 *						}
 *					};
 *
 * 构建代码：
 *  
 *  YCLuaParameter params(myLua->luaState(), "YCParameter");
 *  params.value("x", 0)
 *		  .value("y", 0)
 *		  .enter("z")
 *		      .foreach(YCLuaParameter::YCForEachWrapper<YCAvatarController>(this, &YCAvatarController::foreach))
 *			  .enter("l")
 *				 .value("surname", "liao")
 *				 .value("lastname", "zanxu")
 *			  .leave()
 *		  .leave();
 */
class YCAPI YCLuaParameter
{
#pragma region foreach

public:
	
	//
	// YCForEachWrapper foreach functor 包装器接口
	//
	class YCILuaForEachWrapper
	{
	public:
		
		virtual bool handle(YCLuaParameter& writer) = 0;

	protected:
		YCILuaForEachWrapper() {}
		virtual ~YCILuaForEachWrapper() {}
	};

public:

	//
	// YCForEachWrapper: foreach functor 包装器模版类实现
	//
	template<typename T>
	class YCForEachWrapper : public YCILuaForEachWrapper
	{
	public:

		YCForEachWrapper(T* t, bool (T::*handle)(YCLuaParameter&))
			:handler(t), execute(handle)
		{
			SMART_ASSERT(t != NULL && handle != NULL);
		}

		virtual ~YCForEachWrapper() {}

		virtual bool handle(YCLuaParameter& writer)
		{
			return (handler->*execute)(writer);
		}

	private:
		T* handler;
		bool (T::*execute)(YCLuaParameter&);
	};

#pragma endregion foreach

public:

	//
	// 函数：YCLuaParameter(lua_State* l, const char* name)
	//
	// 目的：设置push到Lua环境中的变量名
	//
	YCLuaParameter(lua_State* l, const char* name);

	~YCLuaParameter();
	
	//
	// 函数：enter(const char* name)
	//
	// 目的：创建一个值项
	//
	YCLuaParameter& enter(const char* name = NULL);

	//
	// 函数：item(int value)
	//
	// 目的：设置Int值
	//
	YCLuaParameter& item(const char* name, int value);

	//
	// 函数：item(const char* value)
	//
	// 目的：设置字符串值
	//
	YCLuaParameter& item(const char* name, const char* value);

	//
	// 函数：item(double value)
	//
	// 目的：设置double值
	//
	YCLuaParameter& item(const char* name, double value);

	//
	// 函数：foreach(YCILuaForEachWrapper& wrapper)
	//
	// 目的：设置子表项
	//
	YCLuaParameter& foreach(YCILuaForEachWrapper& wrapper);

	//
	// 函数：leave()
	//
	// 目的：回到上级表项
	//
	YCLuaParameter& leave();

	//
	// 函数：dump()
	//
	// 目的：dump类内部状态到日志文件
	//
	void dump();

	//
	// 函数：push()
	//
	// 目的：推送C++变量到Lua环境，使在Lua脚本中
	//       可以通过YCParameter前最访问
	//
	void push();

private:

	//
	// 函数：checkStack(int sz)
	//
	// 目的：确保栈上有住够的空间保存c到Lua传递的数据
	//
	bool checkStack(int sz);

private:

	lua_State * L;

	char myLastFlag;

	char myName[16];

	YCLuaParameterHolder* myParameters;

};

#endif