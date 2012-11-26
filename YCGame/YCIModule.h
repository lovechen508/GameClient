#ifndef _INCLUDE_YCIMODULE_H_
#define _INCLUDE_YCIMODULE_H_

#include "YCBasic\YCIDumper.h"

class YCDList;

/*
 * YCIModule : 模块抽象接口类
 *             封装对模块的操作
 *             屏蔽底层module打包类型
 *
 *     --------      load       --------      active      --------
 *    | INITED | <===========> | LOADED | <============> | ACTIVE |
 *     --------     unload      --------     deactive     --------
 *        |                        |                          |
 *        |                        V                          |
 *        |                     --------                      |
 *         ------------------> | FAILED | <-------------------
 *                              --------
 *  Module状态定义：
 *
 *       INITED : 系统启动，读入配置文件，异步下载模块 
 *
 *       LOADED : PRELOAD检查模块是否已下载，加载模块，with接口注入依赖资源
 * 
 *       ACTIVE : 激活模块，开始对外提供服务，如响应鼠标，键盘，网络消息等
 * 
 *       FAILED : ?
 */

class YCXmlReader;

class YCAPI YCIModule : public YCIDumper
{
public:

	// 模块状态定义
	enum {INITED = 1, LOADED = 2, ACTIVE = 3, FAILED = 4};
	
public:

	YCIModule();

	virtual ~YCIModule();

	//
	// 函数：with(const char* depend, void* pointer)
	//
	// 目的：根据模块的声明，实现依赖注入
	//
	virtual void with(const char* depend, void* pointer) = 0;

	//
	// 函数：initialize()
	//
	// 目的：下载完成后的初始化，模块状态转换为INITED
	//
	virtual bool initialize(YCXmlReader& reader) = 0;
	
	//
	// 函数：load()
	//
	// 目的：执行真正的模块加载
	//       注册场景层，注册窗口类，加载资源
	//
	virtual void load() = 0;


	//
	// 函数：active()
	//
	// 目的：激活模块
	//       使模块开始接受请求，提供服务
	//
	virtual void active() = 0;


	//
	// 函数：deactive()
	//
	// 目的：休眠模块
	//       模块不再接受请求，不再对外提供服务
	//
	virtual void deactive() = 0;

	//
	// 函数：load()
	//
	// 目的：执行真正的模块卸载
	//       卸载场景层，卸载窗口类，卸载资源
	//
	virtual void unload() = 0;

	//
	// 函数：finialize()
	//
	// 目的： 退出时的资源析构，模块状态转换为UNINITED
	//
	virtual bool finialize() = 0;

	//
	// 函数：name()
	//
	// 目的：取得模块名称
	//
	const char* name();

	//
	// 函数：version()
	//
	// 目的：取得模块版本
	//
	int version();

	//
	// 函数：status()
	//
	// 目的：取得模块状态
	//
	int status();

	//
	// 函数：depend()
	//
	// 目的：返回模块依赖列表
	//
	YCDList* dependencies();

private:

	//
	// 函数：preload()
	//
	// 目的：加载前的准备工作
	//
	virtual void preload() = 0;

	//
	// 函数：postload（）
	//
	// 目的：加载完成后的附属动作
	//
	virtual void postload() = 0;

	//
	// 函数：preunload()
	//
	// 目的：卸载前的准备工作
	//
	virtual void preunload() = 0;

	//
	// 函数：postunload（）
	//
	// 目的：卸载完成后的附属动作
	//
	virtual void postunload() = 0;

protected:

	int myStatus;

	char* myName;
	int myVersion;

	YCDList* myDependencies;
};

#endif