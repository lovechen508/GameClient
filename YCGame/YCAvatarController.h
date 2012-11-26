#ifndef _INCLUDE_YCAVATARCONTROLLER_H_
#define _INCLUDE_YCAVATARCONTROLLER_H_

#include "YCGraphic\YCIController.h"

class YCLua;

class YCLuaParameter;

class YCAvatarController :
	public YCIController
{
public:

	YCAvatarController(YCUIManager* uiManager, YCLua* lua);

	virtual ~YCAvatarController(void);

	//
	// 函数：handleRequest(YCLuaRequest* context)
	//
	// 目的: 处理窗口发出的请求
	//
	virtual void handleRequest(YCLuaRequest* context);

private:

	//
	// 函数：foreach(YCLuaParameter& parameter);
	//
	// 目的: 测试函数
	//
	bool foreach(YCLuaParameter& parameter);

private:

	YCLua* myLua;

};

#endif

