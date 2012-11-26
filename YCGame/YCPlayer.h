#ifndef _INCLUDE_YCPLAYER_H_
#define _INCLUDE_YCPLAYER_H_

#include "YCCreature.h"
#include "YCObjectDef.h"

#include "YCBasic\YCIFsm.h"

class YCPlayer 
	: public YCCreature
	, public YCIFsm
{
public:
	
	YCPlayer(void);

	virtual ~YCPlayer(void);

	//
	// 函数：update(unsigned int)
	//
	// 目的：更新内部状态
	//
	virtual void update(unsigned int event);

	//
	// 函数：游戏mode操作
	//
	// 目的：设置当前游戏模式
	//
	void setMode(E_PLAY_MODE mode);
	E_PLAY_MODE getMode() const;

	//
	// 函数：游戏玩家级别
	//
	// 目的：设置玩家级别
	//
	void setLevel(unsigned short level);
	unsigned short getLevel() const;

	//
	// 函数：游戏玩家性别
	//
	// 目的：设置玩家性别
	//
	void setSex(E_PLAYER_SEX sex);
	E_PLAYER_SEX getSex() const;

	//
	// 函数：HP操作
	//
	// 目的：设置当前血量
	//
	void setHP(unsigned int hp);
	unsigned int getHP() const;

	//
	// 函数：MaxHP操作
	//
	// 目的：设置最大血量
	//
	void setMaxHP(unsigned int maxHP);
	unsigned int getMaxHP() const;

private:

	//
	// 游戏模式：和平, PK...
	//
	E_PLAY_MODE		myPlayMode; 

	//
	// 基本属性组：
	//
	unsigned short	myLevel;  // 级别
	E_PLAYER_SEX	mySex;    // 性别
	
	//
	// 当前,最大血量
	//
	unsigned int	myMaxHP;
	unsigned int	myCurrentHP;
};

#endif
