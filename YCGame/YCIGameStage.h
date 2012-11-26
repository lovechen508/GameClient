#ifndef _INCLUDE_YCIGAMESTAGE_H_
#define _INCLUDE_YCIGAMESTAGE_H_

#include "YCBasic\YCIFsmState.h"

class YCLua;
class YCGame;
class YCConfig;
class YCNetwork;
class YCGraphic;
class YCUIManager;

/*
 * YCIGameStage : 游戏场景转换接口类
 *
 * 说明：
 *       三个派生场景：登录，选角色，游戏
 */
class YCAPI YCIGameStage 
	: public YCIFsmState
{
public:

	enum {
		LOGIN_STAGE	= 1,
		SELECTROLE_STAGE = 2,
		GAME_STAGE = 3
	};

public:
	
	YCIGameStage(YCGame* game, unsigned int stage);
	
	virtual ~YCIGameStage(void);

	//
	// 函数：stageNo() const
	//
	// 目的：取得状态编号
	//
	unsigned int stageNo() const;

	//
	// 函数：nextState(unsigned int)
	//
	// 目的：是否下一状态
	//
	virtual bool nextState(unsigned int);

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：对象创建调用
	//
	virtual bool initialize(YCConfig& config) = 0;

	//
	// 函数：with(unsigned int id, T*)
	//
	// 目的：设置内部必须数据，YCIFsmTransfer调用
	//      不实现，子类实现
	//
	virtual YCIFsmState* with(unsigned int id, void*);

	//
	// 函数: finalize()
	//
	// 目的：对象析构调用
	//
	virtual void finalize() = 0;

protected:

	//
	// myStage : stage编号
	//
	const unsigned int myStage;

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


};

#endif