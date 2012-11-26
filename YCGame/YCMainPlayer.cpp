#include "YCMainPlayer.h"

#include "YCItem.h"
#include "YCMagic.h"
#include "YCPackage.h"
#include "YCMagicTree.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCMainPlayer::YCMainPlayer(void)
	: myPackage(new YCPackage())
	, myMagics(new YCMagicTree())
{
}


YCMainPlayer::~YCMainPlayer(void)
{
	SAFE_DELETE(myPackage);
	SAFE_DELETE(myMagics);
}

//
// 函数：update(unsigned int)
//
// 目的：更新内部状态
//
void YCMainPlayer::update(unsigned int event)
{

}

//
// 函数：当前经验值操作
//
// 目的：设置，获取当前经验值
//
void YCMainPlayer::setExp(unsigned long long exp)
{
	myCurrentExp = exp;
}

unsigned long long YCMainPlayer::getExp() const
{
	return myCurrentExp;
}

//
// 函数：升级经验值操作
//
// 目的：设置，获取升级经验值
//
void YCMainPlayer::setLevelupExp(unsigned long long exp)
{
	myLevelupExp = exp;
}

unsigned long long YCMainPlayer::getLevelupExp() const
{
	return myLevelupExp;
}

//
// 函数：当前物防下限
//
// 目的：设置，获取物防下限
//
void YCMainPlayer::setLLPD(unsigned short llpd)
{
	myLLPD = llpd;
}

unsigned short YCMainPlayer::getLLPD() const
{
	return myLLPD;
}

//
// 函数：当前物防上限
//
// 目的：设置，获取物防上限
//
void YCMainPlayer::setULPD(unsigned short ulpd)
{
	myULPD = ulpd;
}

unsigned short YCMainPlayer::getULPD() const
{
	return myULPD;
}

//
// 函数：当前物攻下限
//
// 目的：设置，获取物攻下限
//
void YCMainPlayer::setLLPA(unsigned short llpa)
{
	myLLPA = llpa;
}

unsigned short YCMainPlayer::getLLPA() const
{
	return myLLPA;
}

//
// 函数：当前物攻上限
//
// 目的：设置，获取物攻上限
//
void YCMainPlayer::setULPA(unsigned short ulpa)
{
	myULPA = ulpa;
}

unsigned short YCMainPlayer::getULPA() const
{
	return myULPA;
}

//
// 函数：当前魔防下限
//
// 目的：设置，获取魔防下限
//
void YCMainPlayer::setLLMD(unsigned short llmd)
{
	myLLMD = llmd;
}

unsigned short YCMainPlayer::getLLMD() const
{
	return myLLMD;
}

//
// 函数：当前魔防上限
//
// 目的：设置，获取魔防上限
//
void YCMainPlayer::setULMD(unsigned short ulmd)
{
	myULMD = ulmd;
}

unsigned short YCMainPlayer::getULMD() const
{
	return myULMD;
}

//
// 函数：当前魔攻下限
//
// 目的：设置，获取魔攻下限
//
void YCMainPlayer::setLLMA(unsigned short llma)
{
	myLLMA = llma;
}

unsigned short YCMainPlayer::getLLMA() const
{
	return myLLMA;
}

//
// 函数：当前魔攻上限
//
// 目的：设置，获取魔攻上限
//
void YCMainPlayer::setULMA(unsigned short ulma)
{
	myULMA = ulma;
}

unsigned short YCMainPlayer::getULMA() const
{
	return myULMA;
}

//
// 函数：MP操作
//
// 目的：设置当前MP
//
void YCMainPlayer::setMP(unsigned int mp)
{
	if (mp > myMaxMP)
	{
		throw YCException(2002, "YCPlayer::setHP血量超过最大值");
	}

	myCurrentMP = mp;
}

unsigned int YCMainPlayer::getMP() const
{
	return myCurrentMP;
}

//
// 函数：MaxMP操作
//
// 目的：设置最大MP
//
void YCMainPlayer::setMaxMP(unsigned int maxMP)
{
	myMaxMP = maxMP;
}

unsigned int YCMainPlayer::getMaxMP() const
{
	return myMaxMP;
}

//
// 函数：包裹，仓库函数组
//
// 目的：包裹操作
//
void YCMainPlayer::addItem(E_PACKAGE_NO package, YCItem* item, unsigned int index)
{
	if (item == NULL)
	{
		throw YCException(2002, "YCMainPlayer::addItem物品为空");
	}

	//  随身包裹
	if (package == PACKAGE_HANDY)
	{
		myPackage->setItemAt(index, item);
	}
}

void YCMainPlayer::removeItem(E_PACKAGE_NO package, unsigned long long id)
{
	if (package == PACKAGE_HANDY)
	{
		YCItem* item = myPackage->findById(id);
		myPackage->removeItemAt(item->getIndex());

		SAFE_DELETE(item);
	}
}

YCItem* YCMainPlayer::getItem(E_PACKAGE_NO package, unsigned int index)
{
	YCItem* found = NULL;
	if (package == PACKAGE_HANDY)
	{
		found = myPackage->getItemAt(index);
	}

	return found;
}