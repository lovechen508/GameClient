#include "YCIController.h"

#include "YCBasic\YCAssert.h"

//
// ������YCIController(YCUIManager* uiManager)
//
// Ŀ�ģ����첢�趨YCIControllerȫ��ΨһUIManager
//
YCIController::YCIController(YCUIManager* uiManager) 
	: myUIManager(uiManager)
{
	SMART_ASSERT(uiManager != NULL);
}

YCIController::~YCIController() 
{
}