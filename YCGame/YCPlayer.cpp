#include "YCPlayer.h"

#include "YCBasic\YCException.h"

YCPlayer::YCPlayer(void)
{
}


YCPlayer::~YCPlayer(void)
{
}

//
// 函数：update(unsigned int)
//
// 目的：更新内部状态
//
void YCPlayer::update(unsigned int event)
{
	
}

//
// 函数：游戏mode操作
//
// 目的：设置当前游戏模式
//
void YCPlayer::setMode(E_PLAY_MODE mode)
{
	myPlayMode = mode;
}

E_PLAY_MODE YCPlayer::getMode() const
{
	return myPlayMode;
}

//
// 函数：游戏玩家级别
//
// 目的：设置玩家级别
//
void YCPlayer::setLevel(unsigned short level)
{
	myLevel = level;
}

unsigned short YCPlayer::getLevel() const
{
	return myLevel;
}

//
// 函数：游戏玩家性别
//
// 目的：设置玩家性别
//
void YCPlayer::setSex(E_PLAYER_SEX sex)
{
	mySex = sex;
}

E_PLAYER_SEX YCPlayer::getSex() const
{
	return mySex;
}

//
// 函数：HP操作
//
// 目的：设置当前血量
//
void YCPlayer::setHP(unsigned int hp)
{
	if (hp > myMaxHP)
	{
		throw YCException(2002, "YCPlayer::setHP血量超过最大值");
	}

	myCurrentHP = hp;
}

unsigned int YCPlayer::getHP() const
{
	return myCurrentHP;
}

//
// 函数：MaxHP操作
//
// 目的：设置最大血量
//
void YCPlayer::setMaxHP(unsigned int maxHP)
{
	myMaxHP = maxHP;
}

unsigned int YCPlayer::getMaxHP() const
{
	return myMaxHP;
}
