#include "YCPlayer.h"

#include "YCBasic\YCException.h"

YCPlayer::YCPlayer(void)
{
}


YCPlayer::~YCPlayer(void)
{
}

//
// ������update(unsigned int)
//
// Ŀ�ģ������ڲ�״̬
//
void YCPlayer::update(unsigned int event)
{
	
}

//
// ��������Ϸmode����
//
// Ŀ�ģ����õ�ǰ��Ϸģʽ
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
// ��������Ϸ��Ҽ���
//
// Ŀ�ģ�������Ҽ���
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
// ��������Ϸ����Ա�
//
// Ŀ�ģ���������Ա�
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
// ������HP����
//
// Ŀ�ģ����õ�ǰѪ��
//
void YCPlayer::setHP(unsigned int hp)
{
	if (hp > myMaxHP)
	{
		throw YCException(2002, "YCPlayer::setHPѪ���������ֵ");
	}

	myCurrentHP = hp;
}

unsigned int YCPlayer::getHP() const
{
	return myCurrentHP;
}

//
// ������MaxHP����
//
// Ŀ�ģ��������Ѫ��
//
void YCPlayer::setMaxHP(unsigned int maxHP)
{
	myMaxHP = maxHP;
}

unsigned int YCPlayer::getMaxHP() const
{
	return myMaxHP;
}
