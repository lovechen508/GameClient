#ifndef _INCLUDE_YCIMODEL_H_
#define _INCLUDE_YCIMODEL_H_

class YCDList;
class YCIComponent;

/*
 * Model 类 
 *   利用CPP的反射机制实现Model和View的绑定
 *
 *                                                     ---------------------
 *                                                    |      YCIModel       |
 *                                                     ---------------------
 *                                                               |
 *                                                     ---------------------
 *                                                    |      YCISprite      |
 *                                                     ---------------------
 *                                                               |
 *                    ---------------------------------------------------------------------------------------
 *                   |                          |                            |                               |
 *            ---------------             ----------------            -------------                  ------------------
 *           |    YCItem     |           | YCIMessageUnit |          |   YCMagic   |                |    YCMapObject   |
 *            ---------------             ----------------            -------------                  ------------------
 *                                                                                                           |
 *                                                                     ---------------------------------------------------------------------------
 *                                                                    |                                                                           |
 *                                                             ---------------                                                             ---------------
 *                                                            |  YCCreature   |                                                           | YCMagicEffect |
 *                                                             ---------------                                                             ---------------
 *                                                                    |
 *                                 --------------------------------------------------------------------------
 *                                |                                   |                                      |
 *                         --------------                        -----------                          --------------
 *                        |   YCMonster  |                      |   YCNpc   |                        |   YCPlayer   |
 *                         --------------                        -----------                          --------------
 *                                                                                                           |
 *                                                                                                    --------------
 *                                                                                                   | YCMainPlayer |
 *                                                                                                    --------------
 */

class YCAPI YCIModel  
{
public:

	enum E_MODEL_STATE{MODEL_UPDATED, MODEL_DELETE} E_MODEL_STATE;

public:

	YCIModel(void);

	virtual ~YCIModel(void);

	//
	// 函数：initialize()
	//
	// 目的：数据模型初始化
	//
	//virtual bool initialize() = 0;

	//
	// 函数：subscribe(YCIComponent*)
	//
	// 目的：绑定对数据模型的监听
	//       目前的设计中实际上YCIComponent充当了View和Controller两者的角色
	//
	void subscribe(YCIComponent* ui);

	//
	// 函数：unsubscribe(YCIComponent*)
	//
	// 目的：取消对数据模型的监听
	//       目前的设计中实际上YCIComponent充当了View和Controller两者的角色
	//
	void unsubscribe(YCIComponent* ui);

	//
	// 函数：finialize()
	//
	// 目的：释放数据模型中的资源
	//
	//virtual void finialize() = 0;

private:

	YCDList * mySubscribers;
	
};

#endif
