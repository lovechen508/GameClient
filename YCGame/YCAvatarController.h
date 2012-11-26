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
	// ������handleRequest(YCLuaRequest* context)
	//
	// Ŀ��: �����ڷ���������
	//
	virtual void handleRequest(YCLuaRequest* context);

private:

	//
	// ������foreach(YCLuaParameter& parameter);
	//
	// Ŀ��: ���Ժ���
	//
	bool foreach(YCLuaParameter& parameter);

private:

	YCLua* myLua;

};

#endif

