#include "YCMonster.h"

#include "YCBasic/YCException.h"

YCMonster::YCMonster(void)
{
}


YCMonster::~YCMonster(void)
{
}

//
// ������update(unsigned int)
//
// Ŀ�ģ������ڲ�״̬
//
void YCMonster::update(unsigned int event)
{

}

//
// ������HP����
//
// Ŀ�ģ����õ�ǰѪ��
//
void YCMonster::setHP(unsigned int hp)
{
	if (hp > myMaxHP)
	{
		throw YCException(2002, "YCMonster::setHPѪ���������ֵ");
	}

	myCurrentHP = hp;
}

unsigned int YCMonster::getHP() const
{
	return myCurrentHP;
}

//
// ������MaxHP����
//
// Ŀ�ģ��������Ѫ��
//
void YCMonster::setMaxHP(unsigned int maxHP)
{
	myMaxHP = maxHP;
}

unsigned int YCMonster::getMaxHP() const
{
	return myMaxHP;
}
