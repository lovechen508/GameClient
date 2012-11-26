#ifndef _INCLUDE_YCISPRITE_H_
#define _INCLUDE_YCISPRITE_H_

#include <list>

#include "YCIModel.h"

class YCDList;
class YCIAction;

/*
 * YCISprite : 游戏中所有物件的基类
 *
 *          建筑，树木  
 *          人物，怪物，NPC
 *          魔法，技能
 *          天气特效等
 *
 */
class YCAPI YCISprite : public YCIModel
{
public:

	YCISprite(void);

	virtual ~YCISprite(void);

	//
	// 函数：setId(unsigned long long id)
	//
	// 目的：设置唯一ID
	//
	void setId(unsigned long long id);
	unsigned long long getId() const;

	//
	// 函数：draw(unsigned int timestamp)
	//
	// 目的：在地图上完成自身的绘制
	//
	virtual void draw(unsigned int timestamp);

	//
	// 函数：addAction(YCIAction *action)
	//
	// 目的：重置当前动画，添加新动画到队列头部
	//
	void insertAction(YCIAction *action);

	//
	// 函数：appendAction(YCIAction *action)
	//
	// 目的：增加新动画到队列尾
	//
	void appendAction(YCIAction *action);

private:

	//
	// 游戏世界唯一ID
	//
	unsigned long long myID;  
	
	//
	// X, Y坐标
	//
	unsigned int X;	
	unsigned int Y;	

	YCDList* myActions;
};

#endif
