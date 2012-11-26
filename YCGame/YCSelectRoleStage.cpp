#include "YCSelectRoleStage.h"

#include "YCGameStageDefine.h"

#include "YCLua\YCLua.h"
#include "YCBasic\YCAssert.h"
#include "YCGraphic\YCGraphic.h"
#include "YCGraphic\YCUIManager.h"
#include "YCNetwork\YCNetwork.h"

YCSelectRoleStage::YCSelectRoleStage(YCGame* game)
	: YCIGameStage(game, YCIGameStage::SELECTROLE_STAGE)
	, mySessionId(0)
{
}


YCSelectRoleStage::~YCSelectRoleStage(void)
{
}

//
// 函数：initialize(YCConfig& config)
//
// 目的：对象创建调用
//
bool YCSelectRoleStage::initialize(YCConfig& config)
{
	return true;
}

//
// 函数：enter()
//
// 目的：状态进入
//
void YCSelectRoleStage::enter()
{
	mySessionId = 0;
}

//
// 函数：with(unsigned int id, unsigned int sessionId)
//
// 目的：设置sessionId
//
YCIFsmState* YCSelectRoleStage::with(unsigned int id, unsigned int sessionId)
{
	SMART_ASSERT(id == TRANSFER_SESSION);
	mySessionId = sessionId;

	return this;
}

//
// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCSelectRoleStage::handle(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

//
// 函数：leave()
//
// 目的：状态结束
//
void YCSelectRoleStage::leave()
{
	mySessionId = 0;
}

//
// 函数: finalize()
//
// 目的：对象析构调用
//
void YCSelectRoleStage::finalize()
{

}
