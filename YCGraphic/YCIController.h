#ifndef _INCLUDE_YCICONTROLLER_H_
#define _INCLUDE_YCICONTROLLER_H_

#include "YCUIManager.h"

class YCLuaRequest;

/*
 * YCIController : Controller ����
 */
class YCAPI YCIController
{

public:

	//
	// ������YCIController(YCUIManager* uiManager)
	//
	// Ŀ�ģ����첢�趨YCIControllerȫ��ΨһUIManager
	//
	YCIController(YCUIManager* uiManager);

	virtual ~YCIController();

	//
	// ������handleRequest(YCLuaRequest* context)
	//
	// Ŀ��: �����ڷ���������
	//
	virtual void handleRequest(YCLuaRequest* context) = 0;

protected:

	YCUIManager * myUIManager;

};

#endif