#include "YCGameStage.h"

#include "YCGameStageDefine.h"

#include "YCMainPlayer.h"
#include "YCMapFactory.h"
#include "YCChatManager.h"
#include "YCTaskManager.h"
#include "YCBuffManager.h"

#include "YCLua\YCLua.h"
#include "YCBasic\YCLogger.h"
#include "YCGraphic\YCGraphic.h"
#include "YCGraphic\YCUIManager.h"
#include "YCNetwork\YCNetwork.h"

YCGameStage::YCGameStage(YCGame* game)
	: YCIGameStage(game, YCIGameStage::GAME_STAGE)
	, myMapFactory(NULL)
	, myChatManager(NULL)
	, myTaskManager(NULL)
	, mySessionId(0)
	, myRoleId(0)
	, mySelf(NULL)
{
}


YCGameStage::~YCGameStage(void)
{
}

//
// 函数：initialize(YCConfig& config)
//
// 目的：对象创建调用
//
bool YCGameStage::initialize(YCConfig& config)
{
	myMapFactory = new YCMapFactory();
	if (!myMapFactory->initialize(config))
	{
		return false;
	}
	LOG_DEBUG("YCGame::initialize初始化MapFactory成功！");
	
	// 注册地图工厂
	myGraphic->registerMapFactory(myMapFactory);

	myChatManager = new YCChatManager();
	if (!myChatManager->initialize(config))
	{
		return false;
	}
	LOG_DEBUG("YCGame::initialize初始化ChatManager成功！");

	myTaskManager = new YCTaskManager();
	if (!myTaskManager->initialize(config))
	{
		return false;
	}
	LOG_DEBUG("YCGame::initialize初始化TaskManager成功！");

	//myBuffManager = new YCBuffManager();
	//if (!myBuffManager->)

	return true;
}

//
// 函数：enter()
//
// 目的：状态进入
//
void YCGameStage::enter()
{
	SMART_ASSERT(mySelf == NULL);
}

//
// 函数：with(unsigned int id, unsigned int val)
//
// 目的：设置sessionId, roleId
//
YCIFsmState* YCGameStage::with(unsigned int id, unsigned int val)
{
	if (id == TRANSFER_SESSION)
	{
		mySessionId = val;
	}
	else if (id == TRANSFER_ROLEID)
	{
		myRoleId = val;
	}

	return this;
}

//
// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCGameStage::handle(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

//
// 函数：leave()
//
// 目的：状态结束
//
void YCGameStage::leave()
{
	SAFE_DELETE(mySelf);
	mySessionId = 0;
	myRoleId = 0;
}

//
// 函数: finalize()
//
// 目的：对象析构调用
//
void YCGameStage::finalize()
{
	if (myMapFactory != NULL)
	{
		myMapFactory->finalize();
		SAFE_DELETE(myMapFactory);
	}

	if (myChatManager != NULL)
	{
		myChatManager->finalize();
		SAFE_DELETE(myChatManager);
	}

	if (myTaskManager != NULL)
	{
		myTaskManager->finalize();
		SAFE_DELETE(myTaskManager);
	}
}