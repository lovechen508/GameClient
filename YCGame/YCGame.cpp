#include "YCGame.h"

#include "YCModuleManager.h"

#include "YCLoginStage.h"
#include "YCSelectRoleStage.h"
#include "YCGameStage.h"
#include "YCGameStageDefine.h"

#include "YCLua\YCLua.h"
#include "YCBasic\YCDList.h"
#include "YCInput\YCInput.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"
#include "YCConfig\YCConfig.h"
#include "YCGraphic\YCGraphic.h"
#include "YCGraphic\YCUIManager.h"
#include "YCNetwork\YCNetwork.h"

#pragma region createStage

//
// 函数：createStage(T*)
//
// 目的：模版函数，创建游戏Stage
//
template<typename T>
struct YCGameStageHelper
{
	static YCIGameStage* createStage(YCGame *game)
	{
		YCIGameStage* stage = new T(game);
		stage->with(TRANSFER_LUA, game->getLua())
			 ->with(TRANSFER_NETWORK, game->getNetwork())
			 ->with(TRANSFER_GRAPHIC, game->getGraphic())
			 ->with(TRANSFER_UI, game->getUIManager());
		return stage;
	}
};

#pragma endregion createStage

YCGame::YCGame()
	: myLua(NULL)
	, myNetwork(NULL)
	, myGraphic(NULL)
	, myUIManager(NULL)
	, myModuleManager(NULL)
{

}

YCGame::~YCGame()
{
}

//
// 函数组：访问全局状态
//
YCLua* YCGame::getLua() const
{
	return myLua;
}

YCNetwork* YCGame::getNetwork() const
{
	return myNetwork;
}

YCGraphic* YCGame::getGraphic() const
{
	return myGraphic;
}

YCUIManager* YCGame::getUIManager() const
{
	return myUIManager;
}

//
// 函数：initialize(HWND hWnd, YCInput& input, YCConfig& config)
//
// 目的：初始化游戏控制器
//
bool YCGame::initialize(HWND hWnd, YCInput& input, YCConfig& config)
{
	myWnd = hWnd;

	myNetwork = new YCNetwork();
	if (!myNetwork->initialize(myWnd, &input, config))
	{
		return false;
	}
	LOG_DEBUG("YCGame::initialize初始化网络部分成功!");

	myLua = new YCLua();
	if (!myLua->bootstrap())
	{
		return false;
	}
	YCRegistry::set("lua", myLua);
	LOG_DEBUG("YCGame::initialize初始化Lua环境成功!");

	return true;
}

//
// 函数：bindGraphic(YCGraphic* graphic)
//
// 目的：绑定图形引擎
//
void YCGame::bindGraphic(YCGraphic* graphic)
{
	SMART_ASSERT(graphic != NULL);

	if (graphic == NULL)
	{
		throw YCException(2002, "YCGame::bindGraphic绑定空graphic！");
	}
	myGraphic = graphic;

	myUIManager = myGraphic->getUIManager();

	//注册视图控制器
	initializeController();
}

//
// 函数：launchStage(YCConfig& config)
//
// 目的：创建游戏场景
//
void YCGame::launchStage(YCConfig& config)
{
	// 启动游戏状态机
	YCIGameStage* stage = YCGameStageHelper<YCLoginStage>::createStage(this);
	if (!stage->initialize(config))
	{
		throw YCException(2002, "YCGame::launchStage创建登录场景失败");
	}
	addState(stage, true);

	stage = YCGameStageHelper<YCSelectRoleStage>::createStage(this);
	if (!stage->initialize(config))
	{
		throw YCException(2002, "YCGame::launchStage创建角色场景失败");
	}
	addState(stage, false);

	stage = YCGameStageHelper<YCGameStage>::createStage(this);
	if (!stage->initialize(config))
	{
		throw YCException(2002, "YCGame::launchStage创建游戏场景失败");
	}
	addState(stage,false);
}

//
// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCGame::handle(UINT message, WPARAM wParam, LPARAM lParam)
{
	// if () 自身注册有处理器
	// {
	//		
	// }
	// else	
	// {
	//		if (myCurrent != NULL)
	//      {
	// 	        return myCurrent->handle(message, wParam, lParam);
	//      }
	// }

	return false;
}

//
// 函数：finalize()
//
// 目的：释放游戏控制器
//
void YCGame::finalize()
{	
	if (myLua != NULL)
	{
		YCRegistry::remove("lua");
		SAFE_DELETE(myLua);
		myLua = NULL;
	}

	if (myNetwork != NULL)
	{
		SAFE_DELETE(myNetwork);
		myNetwork = NULL;
	}

	if (myModuleManager != NULL)
	{
		SAFE_DELETE(myModuleManager);
		myModuleManager = NULL;
	}

	for (Item_List_T* item = myStates->begin();
		 item != myStates->end();
		 item = myStates->next(item))
	{
		YCIGameStage* stage = (YCIGameStage*)myStates->payload(item);
		try
		{
			stage->finalize();
		}
		catch (...)
		{
			LOG_ERROR("释放游戏场景异常：" << stage->stageNo());
		}
	}
}

/////////////////////////////////////////////////////////////
// Private Part
/////////////////////////////////////////////////////////////

#include "YCAvatarController.h"

//
// 函数：initializeController()
//
// 目的：向YCUIManager注册YCIController
//
void YCGame::initializeController()
{
	SMART_ASSERT(myUIManager != NULL).fatal("界面管理器为NULL，无法注册视图控制器");

	myUIManager->registController("avatar", new YCAvatarController(myUIManager, myLua));
}


