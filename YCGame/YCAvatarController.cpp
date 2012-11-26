#include "YCAvatarController.h"

#include <map>

#include "YCLua\YCLua.h"
#include "YCLua\YCLuaParameter.h"

#include "YCBasic\YCLogger.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

YCAvatarController::YCAvatarController(YCUIManager* uiManager, YCLua* lua)
	: YCIController(uiManager)
	, myLua(lua)
{
}


YCAvatarController::~YCAvatarController(void)
{
}

//
// ������handleRequest(YCLuaRequest* context)
//
// Ŀ��: �����ڷ���������
//
void YCAvatarController::handleRequest(YCLuaRequest* context)
{
	// ��������������ģ���������
	YCLuaParameter params(myLua->luaState(), "YCParameter");
	params.item("name", "����hello��")
		  .item("mainPlayer", 1)
		  .enter("z")
		      .foreach(YCLuaParameter::YCForEachWrapper<YCAvatarController>(this, &YCAvatarController::foreach))
			  .enter("l")
					.item("surname", "liao")
					.item("lastname", "zanxu")
			  .leave()
		  .leave();

	try
	{
		if (!myUIManager->open("avatar", &params))
		{
			LOG_WARNING("YCAvatarController::handleRequest��avatar����ʧ��");
		}
	}
	catch (YCException& e)
	{		
		LOG_WARNING("YCAvatarController::handleRequest�򿪴���ʧ��: " << e.what());
	}
}

//
// ������foreach(YCLuaParameter& parameter);
//
// Ŀ��: ���Ժ���
//
bool YCAvatarController::foreach(YCLuaParameter& parameter)
{
	std::map<int, std::string> map0;
	map0[0] = "X";
	map0[1] = "Y";
	map0[2] = "Z";

	std::map<int, std::string> map1;
	map1[0] = "XX";
	map1[1] = "YY";
	map1[2] = "ZZ";

	for (int i = 0; i < 3; ++i)
	{
		parameter.enter()
			       .item("a", map0[i].c_str())
			       .item("b", map1[i].c_str())
				 .leave();
	}
	return true;
}
