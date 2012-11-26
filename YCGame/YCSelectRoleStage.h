#ifndef _INCLUDE_YCSELECTROLESTAGE_H_
#define _INCLUDE_YCSELECTROLESTAGE_H_

#include "YCIGameStage.h"

/*
 * YCSelectRoleStage : 选角色界面
 */
class YCAPI YCSelectRoleStage :
	public YCIGameStage
{
public:

	YCSelectRoleStage(YCGame* game);

	virtual ~YCSelectRoleStage(void);

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
	// 函数：with(unsigned int id, unsigned int sessionId)
	//
	// 目的：设置sessionId
	//
	virtual YCIFsmState* with(unsigned int id, unsigned int sessionId);

	//
	// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	bool handle(UINT message, WPARAM wParam, LPARAM lParam);

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
};

#endif

