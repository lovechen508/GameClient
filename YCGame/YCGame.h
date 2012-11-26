#ifndef _INCLUDE_YCGAME_H_
#define _INCLUDE_YCGAME_H_

#include "YCIGameStage.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCIFsm.h"

class YCLua;
class YCInput;
class YCConfig;
class YCNetwork;
class YCGraphic;
class YCUIManager;
class YCModuleManager;

/*
 * YCGame : 游戏控制核心类
 *  
 * 注释：
 *     网络连接 YCNetwork
 *     游戏数据 YCData
 *
 *                          Launcher                            ||                            GameWorld
 *																||
 *                     --------------------                     ||
 *                    |                    |                    ||
 *                    |                    |                    ||
 *                    |                    |                    ||                            Failed                            CreateRole
 *                    |   GameNew, Forum   |                    ||                        --------------                      --------------
 *                    |    Game Update     |                    ||                       |              |                    |              |
 *                    |                    |                    ||                       V              |                    V              |
 *                    |                    |                    ||                 ---------------      |		      ---------------       |			  ---------------  
 *                    |                    |      IP、Port      ||                |			      |-----		     |			      |-----			 |			      |
 *                    |    Game Section    | -------------------||--------------> |     Login     | ---------------> |   SelectRole   | ---------------> |    GameWorld   |
 *	     Launch       |    ------------    |                    ||                |				  |   successful     |				  |   successful     |				  |
 *	----------------->|                    |                    ||                 ---------------				      ---------------					  ---------------
 *		              |                    |                    || 						  |									|                                    |
 *		              |                    |                    ||						  |									|                                    |
 *		              |    Game Portal     |                    ||						  |									|               ReselectRole		 |
 *		              |    (IP, Port)      |                    ||						  |									<------------------------------------|
 *		              |                    |                    ||						  |																		 |
 *		              |                    |                    ||						  |                          Logout										 |
 *		              |                    |                    ||						   <---------------------------------------------------------------------|
 *		              |      AntiVirs      |                    ||																								 |
 *		              |                    |                    ||						               Back to choose Game Section                               |
 *		              |                    |<-------------------||-----------------------------------------------------------------------------------------------|
 *		              |                    |                    ||
 *                     --------------------                     ||
 *     ...
 */
class YCAPI YCGame
	: public YCIFsm
{
public:

	YCGame();

	~YCGame();

	//
	// 函数组：访问全局状态
	//
	YCLua* getLua() const;
	YCNetwork* getNetwork() const;
	YCGraphic* getGraphic() const;
	YCUIManager* getUIManager() const;

	//
	// 函数：initialize(HWND hWnd, YCInput& input, YCConfig& config)
	//
	// 目的：初始化游戏控制器
	//
	bool initialize(HWND hWnd, YCInput& input, YCConfig& config);

	//
	// 函数：bindGraphic(YCGraphic* graphic)
	//
	// 目的：绑定图形引擎
	//
	void bindGraphic(YCGraphic* graphic);

	//
	// 函数：launchStage(YCConfig& config)
	//
	// 目的：创建游戏场景
	//
	void launchStage(YCConfig& config);

	//
	// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// 函数：finalize()
	//
	// 目的：释放游戏控制器
	//
	void finalize();

private:

	//
	// 函数：initializeController()
	//
	// 目的：向YCUIManager注册YCIController
	//
	void initializeController();

private:

	HWND myWnd;

	//
	// Lua环境句柄
	//
	YCLua *myLua;

	//
	// 网络接口管理器，持有生命周期
	//
	YCNetwork *myNetwork;

	//
	// 图形接口，不持有生命周期
	//
	YCGraphic *myGraphic;

	//
	// 界面管理器，不持有生命周期
	//
	YCUIManager *myUIManager;
	
	//
	// 模块管理器，持有生命周期
	//
	YCModuleManager* myModuleManager;
};

#endif