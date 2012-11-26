#ifndef _INCLUDE_YCLUASTACK_H_
#define _INCLUDE_YCLUASTACK_H_

#include "YCBasic\YCAssert.h"

struct lua_State;

class YCStack;
class YCLuaParameterHolder;

/*
 * YCLuaParameter : 
 *              ��Ϸ�к���UI����ʱ����ֵ��Lua����
 *              DLS ����API���˼·
 *
 * ˵����
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
 * �������룺
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
	// YCForEachWrapper foreach functor ��װ���ӿ�
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
	// YCForEachWrapper: foreach functor ��װ��ģ����ʵ��
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
	// ������YCLuaParameter(lua_State* l, const char* name)
	//
	// Ŀ�ģ�����push��Lua�����еı�����
	//
	YCLuaParameter(lua_State* l, const char* name);

	~YCLuaParameter();
	
	//
	// ������enter(const char* name)
	//
	// Ŀ�ģ�����һ��ֵ��
	//
	YCLuaParameter& enter(const char* name = NULL);

	//
	// ������item(int value)
	//
	// Ŀ�ģ�����Intֵ
	//
	YCLuaParameter& item(const char* name, int value);

	//
	// ������item(const char* value)
	//
	// Ŀ�ģ������ַ���ֵ
	//
	YCLuaParameter& item(const char* name, const char* value);

	//
	// ������item(double value)
	//
	// Ŀ�ģ�����doubleֵ
	//
	YCLuaParameter& item(const char* name, double value);

	//
	// ������foreach(YCILuaForEachWrapper& wrapper)
	//
	// Ŀ�ģ������ӱ���
	//
	YCLuaParameter& foreach(YCILuaForEachWrapper& wrapper);

	//
	// ������leave()
	//
	// Ŀ�ģ��ص��ϼ�����
	//
	YCLuaParameter& leave();

	//
	// ������dump()
	//
	// Ŀ�ģ�dump���ڲ�״̬����־�ļ�
	//
	void dump();

	//
	// ������push()
	//
	// Ŀ�ģ�����C++������Lua������ʹ��Lua�ű���
	//       ����ͨ��YCParameterǰ�����
	//
	void push();

private:

	//
	// ������checkStack(int sz)
	//
	// Ŀ�ģ�ȷ��ջ����ס���Ŀռ䱣��c��Lua���ݵ�����
	//
	bool checkStack(int sz);

private:

	lua_State * L;

	char myLastFlag;

	char myName[16];

	YCLuaParameterHolder* myParameters;

};

#endif