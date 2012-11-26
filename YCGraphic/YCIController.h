#ifndef _INCLUDE_YCICONTROLLER_H_
#define _INCLUDE_YCICONTROLLER_H_

#include "YCUIManager.h"

class YCLuaRequest;

/*
 * YCIController : Controller 基类
 */
class YCAPI YCIController
{

public:

	//
	// 函数：YCIController(YCUIManager* uiManager)
	//
	// 目的：构造并设定YCIController全局唯一UIManager
	//
	YCIController(YCUIManager* uiManager);

	virtual ~YCIController();

	//
	// 函数：handleRequest(YCLuaRequest* context)
	//
	// 目的: 处理窗口发出的请求
	//
	virtual void handleRequest(YCLuaRequest* context) = 0;

protected:

	YCUIManager * myUIManager;

};

#endif