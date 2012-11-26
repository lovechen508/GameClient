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
// ������initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
//
// Ŀ�ģ���ʼ���ڲ����ݽṹ
//       ��ʼ��Http�������ӳ�     
//       ����LRU�����߳�
//
bool YCAnimationManager::initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
{
	return true;
}

//
// ������finalize()
//
// Ŀ�ģ�ֹͣLRU�����߳�  
//       �ͷ��ڲ����ݽṹ
//
void YCAnimationManager::finalize()
{

}

//
// ������getAnimation(const char* filename)
//
// Ŀ�ģ����ļ�����������
//
YCIAnimation* YCAnimationManager::getAnimation(const char* filename)
{
	throw YCException(2002, "YCAnimationManager::getAnimation��δʵ�֣�");

	return NULL;
}

//
// ������getAnimation(E_ANIMATION_TYPE type, int animationId, int direction);
//
// Ŀ�ģ����㶯��ΨһID��������AnimationCache����Ϊ����
//
// ע�ͣ�
//      type �����󶯻�����ħ����Ч
//      direction �� �����ķ���
//                   ���ڶ��󶯻���ֻ�а˸����� ��          0
//                                                     7     1
//                                                  6           2
//                                                     5     3
//                                                        4
//                   ����ħ����Ч������û�з���Ҳ������ 0~15������
//      animation :  �ö����ڸ�����еı��
//
//
YCIAnimation* YCAnimationManager::getAnimation(E_ANIMATION_TYPE type, int animationId, int direction)
{
	throw YCException(2002, "YCAnimationManager::getAnimation��δʵ�֣�");

	return NULL;
}
