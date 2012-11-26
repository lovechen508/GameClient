#ifndef _INCLUDE_YCANIMATIONMANAGER_H_
#define _INCLUDE_YCANIMATIONMANAGER_H_

class YCConfig;
class YCIAnimation;
class YCNetworkFileLoader;
class YCAnimationManagerImpl;

/*
 * YCAnimationManager : 动画管理器
 */
class YCAnimationManager
{
public:
		
	/*
	 * E_ANIMATION_TYPE : 动画类型枚举定义
	 *
	 * 注释：
	 *
	 *		目前就分成两种：
	 *          1. 对象动画  Player, Monster, NPC行为，攻击等
	 *          2. 魔法特效  Effect，相对对象动画，属性更复杂
	 */
	enum E_ANIMATION_TYPE {
		OBJECT_ANIMATION = 1,
		EFFECT_ANIMATION = 2
	};
	

	YCAnimationManager(void);

	~YCAnimationManager(void);

	//
	// 函数：initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
	//
	// 目的：初始化内部数据结构
	//       初始化Http网络连接池     
	//       启动LRU监视线程
	//
	bool initialize(YCNetworkFileLoader* networkLoader, YCConfig& config);

	//
	// 函数：finalize()
	//
	// 目的：停止LRU监视线程  
	//       释放内部数据结构
	//
	void finalize();

	//
	// 函数：getAnimation(const char* filename)
	//
	// 目的：从文件名构建动画
	//
	YCIAnimation* getAnimation(const char* filename);
	
	//
	// 函数：getAnimation(E_ANIMATION_TYPE type, int animationId, int direction);
	//
	// 目的：计算动画唯一ID，用于在AnimationCache中作为索引
	//
	// 注释：
	//      type ：对象动画还是魔法特效
	//      direction ： 动画的方向
	//                   对于对象动画，只有八个方向 ：          0
	//                                                     7     1
	//                                                  6           2
	//                                                     5     3
	//                                                        4
	//                   对于魔法特效，可能没有方向，也可能有 0~16个方向
	//      animation :  该动画在该类别中的编号
	//
	//
	YCIAnimation* getAnimation(E_ANIMATION_TYPE type, int animationId, int direction);

private:

	YCAnimationManagerImpl * myAnimationManagerImpl;
};

#endif

