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
// 函数：handleRequest(YCLuaRequest* context)
//
// 目的: 处理窗口发出的请求
//
void YCAvatarController::handleRequest(YCLuaRequest* context)
{
	// 根据请求的上下文，构造数据
	YCLuaParameter params(myLua->luaState(), "YCParameter");
	params.item("name", "廖赞hello旭")
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
			LOG_WARNING("YCAvatarController::handleRequest打开avatar窗口失败");
		}
	}
	catch (YCException& e)
	{		
		LOG_WARNING("YCAvatarController::handleRequest打开窗口失败: " << e.what());
	}
}

//
// 函数：foreach(YCLuaParameter& parameter);
//
// 目的: 测试函数
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
