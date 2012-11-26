#ifndef _INCLUDE_YCLUACONTEXT_H_
#define _INCLUDE_YCLUACONTEXT_H_

class YCDList;
class YCLuaContextImpl;

/*
 * YCLuaContext : 设置Lua调用的上下文
 *
 * 说明：
 *      每次从C++环境调用Lua时准备的上下文
 *      比如压入当前窗口的指针，container以便selector选择器可以工作
 *      当前Tag节点，以便this特殊选择器可以工作
 *      压入YCLuaParameter 到 "YCParameter", 以便YCParameter可以取到相关数据
 *
 *      YCLuaContext被压入到YCLuaStack栈中，以便C++到Lua的重入
 *      通用的调用流程是：
 *      
 *      C++                                               =》                              Lua
 *
 *      // YCIContainer.cpp                                                          
 *      YCLuaContext context;
 *      context.set("container", this); //填充context
 *      context.set("this",      this);
 *      context.set("parameter", YCLuaParameter);
 *      YCLua->execute(&context, luaCode);
 *           |--> YCLuaStack->push(context);                              
 *           |--> lua_pcall --------------------------------------------------------->YCQuery(this).close()              
 *                                                                                          |
 *                                   // YCQuery.cpp                                         |
 *                                   YCLuaContext* context = YCLuaStack->front(); <---------|
 *                                   YCIUITag* uiTag = (YCIUITag*)context->get("this")
 *                                   uiTag->close();
 *                                         |
 *                                         |----------------------------------------------->|
 *                                                                                          |
 *           |-- YCLuaStack->pop() <--------------------------------------------------------|
 *           |
 *           o    
 *
 *  注意：容器内的context都不持有生命周期
 *       调用者必须自己释放
 */
class YCAPI YCLuaContext
{
public:

	YCLuaContext(void);

	~YCLuaContext(void);

	//
	// 函数：addContext(const char* key, void* context)
	//
	// 目的：设定一个上下文值
	//
	void addContext(const char* key, void* context);

	//
	// 函数：removeContext(const char* key);
	//
	// 目的：改变一个上下文值
	//
	void removeContext(const char* key);
	
	//
	// 函数：updateSelector(YCDList* selector);
	//
	// 目的：改变一个由YCQuery.selector遴选出的YCUITag节点列表
	//
	void updateSelector(YCDList* selector);
	
	//
	// 函数：lookup(const char* key)
	//
	// 目的：查询一个上下文值
	//
	void* lookup(const char* key);

private:

	YCLuaContextImpl *myContextImpl;
};

#endif

