#ifndef _INCLUDE_YCGAMESTAGE_H_
#define _INCLUDE_YCGAMESTAGE_H_

#include "YCIGameStage.h"

class YCMainPlayer;

class YCGame;
class YCGraphic;
class YCUIManager;
class YCMapFactory;
class YCChatManager;
class YCTaskManager;
class YCBuffManager;

class YCAPI YCGameStage :
	public YCIGameStage
{
public:

	YCGameStage(YCGame* game);

	virtual ~YCGameStage(void);

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：对象创建调用
	//
	virtual bool initialize(YCConfig& config);

	//
	// 函数：enter()
	//
	// 目的：状态进入
	//
	virtual void enter();

	//
	// 函数：with(unsigned int id, unsigned int val)
	//
	// 目的：设置sessionId, roleId
	//
	virtual YCIFsmState* with(unsigned int id, unsigned int val);

	//
	// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// 函数：leave()
	//
	// 目的：状态结束
	//
	virtual void leave();

	//
	// 函数: finalize()
	//
	// 目的：对象析构调用
	//
	virtual void finalize();

private:
	
	//
	// 回话ID
	//
	unsigned int mySessionId;

	//
	// 角色ID
	//
	unsigned int myRoleId;

	//
	// 主玩家
	//
	YCMainPlayer* mySelf;
	
	//
	// 地图管理器，持有生命周期
	//
	YCMapFactory* myMapFactory;

	//
	// 聊天管理器，持有生命周期
	//
	YCChatManager* myChatManager;

	//
	// 任务管理器：持有生命周期
	//
	YCTaskManager* myTaskManager;

	//
	// Buff管理器：持有生命周期
	//
	YCBuffManager* myBuffManager;

};

#endif

