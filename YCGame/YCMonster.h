#ifndef _INCLUDE_YCMONSTER_H_
#define _INCLUDE_YCMONSTER_H_

#include "YCCreature.h"

#include "YCBasic\YCIFsm.h"

/*
 * YCMonster : 怪物对象类
 *
 * 说明：
 *          怪物状态转换
 *
 *                     idle    :   原地发呆
 *                     move    ：  移动，跟踪
 *                     attack  ：  攻击
 *                     escape  ：  逃跑
 *
 *                     --------------                        --------------
 *                    |     Idle     | <==================> |     Move     |
 *                     --------------                        --------------
 *                           |                                     |
 *                           |                                     |
 *                           |                                     |
 *                     --------------                        --------------
 *                    |    Attack    | <==================> |    Escape    |
 *                     --------------                        --------------
 *
 */
class YCMonster 
	: public YCCreature
	, public YCIFsm
{
public:

	YCMonster(void);

	virtual ~YCMonster(void);

	//
	// 函数：update(unsigned int)
	//
	// 目的：更新内部状态
	//
	virtual void update(unsigned int event);

public:

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
	// 最大血量
	//
	unsigned int myMaxHP;

	//
	// 当前血量
	//
	unsigned int myCurrentHP;


};

#endif

