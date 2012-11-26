#ifndef _INCLUDE_YCANIMATIONMANAGER_H_
#define _INCLUDE_YCANIMATIONMANAGER_H_

class YCConfig;
class YCIAnimation;
class YCNetworkFileLoader;
class YCAnimationManagerImpl;

/*
 * YCAnimationManager : ����������
 */
class YCAnimationManager
{
public:
		
	/*
	 * E_ANIMATION_TYPE : ��������ö�ٶ���
	 *
	 * ע�ͣ�
	 *
	 *		Ŀǰ�ͷֳ����֣�
	 *          1. ���󶯻�  Player, Monster, NPC��Ϊ��������
	 *          2. ħ����Ч  Effect����Զ��󶯻������Ը�����
	 */
	enum E_ANIMATION_TYPE {
		OBJECT_ANIMATION = 1,
		EFFECT_ANIMATION = 2
	};
	

	YCAnimationManager(void);

	~YCAnimationManager(void);

	//
	// ������initialize(YCNetworkFileLoader* networkLoader, YCConfig& config)
	//
	// Ŀ�ģ���ʼ���ڲ����ݽṹ
	//       ��ʼ��Http�������ӳ�     
	//       ����LRU�����߳�
	//
	bool initialize(YCNetworkFileLoader* networkLoader, YCConfig& config);

	//
	// ������finalize()
	//
	// Ŀ�ģ�ֹͣLRU�����߳�  
	//       �ͷ��ڲ����ݽṹ
	//
	void finalize();

	//
	// ������getAnimation(const char* filename)
	//
	// Ŀ�ģ����ļ�����������
	//
	YCIAnimation* getAnimation(const char* filename);
	
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
	//                   ����ħ����Ч������û�з���Ҳ������ 0~16������
	//      animation :  �ö����ڸ�����еı��
	//
	//
	YCIAnimation* getAnimation(E_ANIMATION_TYPE type, int animationId, int direction);

private:

	YCAnimationManagerImpl * myAnimationManagerImpl;
};

#endif

