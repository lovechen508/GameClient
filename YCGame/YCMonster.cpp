#include "YCMonster.h"

#include "YCBasic/YCException.h"

YCMonster::YCMonster(void)
{
}


YCMonster::~YCMonster(void)
{
}

//
// 函数：update(unsigned int)
//
// 目的：更新内部状态
//
void YCMonster::update(unsigned int event)
{

}

//
// 函数：HP操作
//
// 目的：设置当前血量
//
void YCMonster::setHP(unsigned int hp)
{
	if (hp > myMaxHP)
	{
		throw YCException(2002, "YCMonster::setHP血量超过最大值");
	}

	myCurrentHP = hp;
}

unsigned int YCMonster::getHP() const
{
	return myCurrentHP;
}

//
// 函数：MaxHP操作
//
// 目的：设置最大血量
//
void YCMonster::setMaxHP(unsigned int maxHP)
{
	myMaxHP = maxHP;
}

unsigned int YCMonster::getMaxHP() const
{
	return myMaxHP;
}
