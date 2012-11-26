#include "YCIController.h"

#include "YCBasic\YCAssert.h"

//
// 函数：YCIController(YCUIManager* uiManager)
//
// 目的：构造并设定YCIController全局唯一UIManager
//
YCIController::YCIController(YCUIManager* uiManager) 
	: myUIManager(uiManager)
{
	SMART_ASSERT(uiManager != NULL);
}

YCIController::~YCIController() 
{
}