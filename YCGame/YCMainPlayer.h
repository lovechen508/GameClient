#ifndef _INCLUDE_YCMAINPLAYER_H_
#define _INCLUDE_YCMAINPLAYER_H_

#include "YCPlayer.h"

class YCItem;
class YCIPackage;
class YCMagicTree;

/*
 * YCMainPlayer : 主玩家
 *
 * 说明：
 *
 *
 */
class YCMainPlayer :
	public YCPlayer
{
public:
	
	YCMainPlayer(void);

	virtual ~YCMainPlayer(void);

	//
	// 函数：update(unsigned int)
	//
	// 目的：更新内部状态
	//
	void update(unsigned int event);

	//
	// 函数：当前经验值操作
	//
	// 目的：设置，获取当前经验值
	//
	void setExp(unsigned long long exp);
	unsigned long long getExp() const;

	//
	// 函数：升级经验值操作
	//
	// 目的：设置，获取升级经验值
	//
	void setLevelupExp(unsigned long long exp);
	unsigned long long getLevelupExp() const;

	//
	// 函数：当前物防下限
	//
	// 目的：设置，获取物防下限
	//
	void setLLPD(unsigned short llpd);
	unsigned short getLLPD() const;

	//
	// 函数：当前物防上限
	//
	// 目的：设置，获取物防上限
	//
	void setULPD(unsigned short ulpd);
	unsigned short getULPD() const;

	//
	// 函数：当前物攻下限
	//
	// 目的：设置，获取物攻下限
	//
	void setLLPA(unsigned short llpa);
	unsigned short getLLPA() const;

	//
	// 函数：当前物攻上限
	//
	// 目的：设置，获取物攻上限
	//
	void setULPA(unsigned short ulpa);
	unsigned short getULPA() const;

	//
	// 函数：当前魔防下限
	//
	// 目的：设置，获取魔防下限
	//
	void setLLMD(unsigned short llmd);
	unsigned short getLLMD() const;

	//
	// 函数：当前魔防上限
	//
	// 目的：设置，获取魔防上限
	//
	void setULMD(unsigned short ulmd);
	unsigned short getULMD() const;

	//
	// 函数：当前魔攻下限
	//
	// 目的：设置，获取魔攻下限
	//
	void setLLMA(unsigned short llma);
	unsigned short getLLMA() const;

	//
	// 函数：当前魔攻上限
	//
	// 目的：设置，获取魔攻上限
	//
	void setULMA(unsigned short ulma);
	unsigned short getULMA() const;

	//
	// 函数：MP操作
	//
	// 目的：设置当前MP
	//
	void setMP(unsigned int mp);
	unsigned int getMP() const;

	//
	// 函数：MaxMP操作
	//
	// 目的：设置最大MP
	//
	void setMaxMP(unsigned int maxMP);
	unsigned int getMaxMP() const;

	//
	// 函数：包裹，仓库函数组
	//
	// 目的：包裹操作
	//
	void addItem(E_PACKAGE_NO package, YCItem* item, unsigned int index);
	void removeItem(E_PACKAGE_NO package, unsigned long long id);
	YCItem* getItem(E_PACKAGE_NO package, unsigned int index);

	//
	// 函数：技能函数组
	//
	// 目的：技能操作
	//
	//void addMagic(YCMagic*);
	//YCMagic* getMagic()


private:

	//
	// 当前,升级所需经验
	//
	unsigned long long myCurrentExp;
	unsigned long long myLevelupExp;

	//
	// 当前,最大MP
	//
	unsigned int myMaxMP;
	unsigned int myCurrentMP;

	//
	// 物理防御 Physical defense
	//
	//  LL : lower limit
	//  UL : upper limit
	//
	unsigned short myLLPD;
	unsigned short myULPD;

	//
	// 物理攻击 Physical attack
	//
	unsigned short myLLPA;
	unsigned short myULPA;

	//
	// 魔防
	//
	unsigned short myLLMD;
	unsigned short myULMD;

	//
	// 魔攻
	//
	unsigned short myLLMA;
	unsigned short myULMA;

	//
	// 包裹
	//
	YCIPackage * myPackage;

	//
	// 技能树
	//
	YCMagicTree * myMagics;
};

#endif

