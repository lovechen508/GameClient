#include "YCAnimationManager.h"

#include <list>
#include <map>

#include "YCBasic\YCException.h"
#include "YCConfig\YCConfig.h"

#include "YCNetworkFileLoader.h"

#pragma region YCAnimationManagerImpl

class YCAnimationManagerImpl
{
public:

	YCAnimationManagerImpl()
	{
	}

	~YCAnimationManagerImpl()
	{
	}



private:

	typedef std::list<YCIAnimation*> AnimationList;
	typedef std::map<unsigned int, AnimationList> AnimationMap;

	AnimationMap * myAnimations;

};

#pragma endregion YCAnimationManagerImpl


YCAnimationManager::YCAnimationManager(void)
{
}


YCAnimationManager::~YCAnimationManager(void)
{
}

//
// 函数：initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
//
// 目的：初始化内部数据结构
//       初始化Http网络连接池     
//       启动LRU监视线程
//
bool YCAnimationManager::initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
{
	return true;
}

//
// 函数：finalize()
//
// 目的：停止LRU监视线程  
//       释放内部数据结构
//
void YCAnimationManager::finalize()
{

}

//
// 函数：getAnimation(const char* filename)
//
// 目的：从文件名构建动画
//
YCIAnimation* YCAnimationManager::getAnimation(const char* filename)
{
	throw YCException(2002, "YCAnimationManager::getAnimation尚未实现！");

	return NULL;
}

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
//                   对于魔法特效，可能没有方向，也可能有 0~15个方向
//      animation :  该动画在该类别中的编号
//
//
YCIAnimation* YCAnimationManager::getAnimation(E_ANIMATION_TYPE type, int animationId, int direction)
{
	throw YCException(2002, "YCAnimationManager::getAnimation尚未实现！");

	return NULL;
}
