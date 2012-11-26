#ifndef _INCLUDE_YCLOGINSTAGE_H_
#define _INCLUDE_YCLOGINSTAGE_H_

#include "YCIGameStage.h"

/*
 * YCLoginStage : 登录阶段
 */
class YCAPI YCLoginStage :
	public YCIGameStage
{
public:

	YCLoginStage(YCGame* game);

	virtual ~YCLoginStage(void);

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：对象创建调用
	//
	virtual bool initialize(YCConfig& config);
	
	//
	// 函数：with(const char* ip)
	//
	// 目的：登录服务器IP
	//
	virtual YCIFsmState* with(unsigned int id, const char* ip);

	//
	// 函数：with(unsigned int id, int port)
	//
	// 目的：登录服务器Port
	//
	virtual YCIFsmState* with(unsigned int id, int port);

	//
	// 函数：enter()
	//
	// 目的：状态进入
	//
	virtual void enter();

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
	// 函数：login(const char* username, const char* password)
	//
	// 目的：从LoginServer登录
	//
	// 注释：阻塞函数，知道服务器返回或者超时
	//       登录成功，则设置mySessionId
	//
	bool login(const char* username, const char* password);

private:

	//
	// loginServer IP
	//
	char* myHost;
	
	//
	// loginServer port
	//
	int myPort;

	//
	// 回话ID
	//
	unsigned int mySessionId;

};

#endif

