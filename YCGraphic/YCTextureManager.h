#ifndef _INCLUDE_YCTEXTUREMANAGER_H_
#define _INCLUDE_YCTEXTUREMANAGER_H_

#include "YCBasic\YCDList.h"
#include "YCBasic\YCIDumper.h"
#include "YCBasic\YCIRunnable.h"
#include "YCBasic\YCThread.h"
#include "YCBasic\YCRWLock.h"

#include "YCTextureDef.h"

class YCTexture;
class YCCondition;

class YCConfig;
class YCITexture2D;
class YCTextureFactory;
class YCNetworkFileLoader;
class YCTexturePoolImpl;

#pragma region description

/*
 * YCTextureManager: ���������
 *
 * ע�ͣ�����������ڴ��й�����̭�㷨
 *       ����������أ���������YCTextureFactory�����
 *       �����������ϵͳ������ֻ��¶YCTextureManager��YCTexture
 *       �Կͻ��˶��ԣ�YCTexture YCTextureManager::fetch(E_TEXTURE_TYPE type, const char* filename) ����ȫ������
 *
 *           ----------------     TexturePool desc
 *          |                |    
 *          |                | ->     1       TileMap  ->��YCITexture2D*��->��YCITexture2D*��->��YCITexture2D*��-> ...
 *          |                |
 *          |                | ->     2       SmallMap ->��YCITexture2D*��->��YCITexture2D*��->��YCITexture2D*��-> ...
 *          |                |
 *          |                | ->     3       Obstacle ->��YCITexture2D*��->��YCITexture2D*��->��YCITexture2D*��-> ...
 *  acquire |                |
 *  <-------|                | ->     4       NPC      ->��YCITexture2D*��->��YCITexture2D*��->��YCITexture2D*��-> ...
 *          |                |
 *          |YCTextureManager| ->     5       Player   ->��YCITexture2D*��->��YCITexture2D*��->��YCITexture2D*��-> ...
 *  release |                | 
 *  ------->|                | ->     6       Monster  ->��YCITexture2D*��->��YCITexture2D*��->��YCITexture2D*��-> ...
 *          |                |
 *          |                | ->     7       Effect   ->��YCITexture2D*��->��YCITexture2D*��->��YCITexture2D*��-> ...
 *          |                |
 *          |                | ->     8       UI       ->��YCITexture2D*��->��YCITexture2D*��->��YCITexture2D*��-> ...
 *          |                |
 *          |                | ->     9       ...
 *          |                |
 *           ----------------
 *
 *  �������ļ�Ŀ¼���еķֲ���
 *
 *   ��Ŀ¼    ���              TextureID
 *    data -> effect                6
 *      |
 *      |
 *      |
 *      |
 *      |
 *      |---> map(Tiled/SmallMap)   1                
 *      |
 *      |
 *      |
 *      |
 *      |
 *      |---> sound                
 *      |
 *      |
 *      |
 *      |
 *      |
 *      |---> ui                    7
 *      |
 *      |
 *      |
 *      |
 *      |---> object -> monster     5
 *              |
 *              |
 *              |
 *              |
 *              |
 *              |
 *              |-----> npc         3
 *              |
 *              |
 *              |
 *              |
 *              |
 *              |
 *              |-----> obstacle    2
 *              |
 *              |
 *              |
 *              |
 *              |
 *              |
 *              |
 *              |-----> player      4
 *
 *
 *
 *              
 *			              ------------------                                  ------------------
 *                       |                  |          has_a                 |                  |                                                  
 *            YCTexture  |                  | -----------------------------> |                  |                ------------------   
 *	 Client <----------- | YCTextureManager |       onLoad(uniqueId)         | YCTextureFactory | ------------> | YCITextuerLoader |              
 *                       |                  | <----------------------------- |                  |                ------------------        
 *                       |                  |                                |                  |                         |
 *			              ------------------                                  ------------------                          |
 *			                      |                                                                                       |
 *			                      |                                                                         ----------------------------
 *			                      |                                                                        |                            |                               
 *			                      |                                                                        |                            |                               
 *			                      |                                                              ---------------------       ---------------------        
 *			                      |                                                             | YCFileTextureLoader |     | YCNetworkFileLoader |      
 *			                      |                                                              ---------------------       ---------------------        
 *			                      |
 *			                      | has_a
 *			                      |
 *			                      |
 *			             ------------------                            --------------  
 *			            |    TexturePool   |------------------------->| YCITexture2D | 
 *			             ------------------                            --------------  
 *			                                                                  |
 *			                                                                  |
 *			                                         ---------------------------------------------------            
 *			                                        |                                                   |
 *			                                        |                                                   |
 *                                         -------------------                                  ------------------
 *                                        | YCSingleTexture2D |                                | YCAtlasTexture2D |
 *                                         -------------------                                  ------------------
 * 
 */

#pragma endregion description

class YCAPI YCTextureManager 
	: public YCIDumper
	, private YCIRunnable
{
	struct scheduledItem {
		int actionType;   //0: ����, 1: shrink, 2: purge
		int parameter;    // ����ر��, 0��ʾȫ��
	};

	typedef enum PoolStatus{STOPPED = 0, STARTED = 1} E_POOL_STATUS;

public:

	YCTextureManager();

	~YCTextureManager();

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
	// Ŀ�ģ��ͷ�Http�������ӳ�   
	//       ֹͣLRU�����߳�  
	//       �ͷ��ڲ����ݽṹ
	//
	void finalize();

	//
	// ������fetch(E_TEXTURE_TYPE type, const chra* filename)
	//
	// Ŀ�ģ���ȡ����
	//      �ͻ�Ψһ��������ӿ�
	//
	// ������
	//		filename : ���·����
	//
	// ע�ͣ�
	//      �ͻ����뱣֤�������͵���ȷ��uniqueId��Ψһ�ԣ����ڱ��ػ������
	//      ������ػ������У�ֱ�ӷ��ض�Ӧ����
	//      ��������YCTextureFactory��ȡ����
	//		    YCTextureFactory:   �����ӦĿ¼�ļ���δ���أ�����YCTexture2DFuture,ͬʱ��YCINetworkLoader�ύ��������
	//                              �����ӦĿ¼���ļ����ڣ����벢����YCSingleTexture2D
	//                              �����ӦĿ¼�´���TextureManager���ɵ�XML Altas�ļ������벢����YCAtlasTexture2D
	//
	YCTexture* fetch(E_TEXTURE_TYPE type, const char* filename);

	//
	// ������update(E_TEXTURE_TYPE type, YCITexture2D* texture);
	//
	// Ŀ�ģ�����YCTextureFactory���أ���������ɹ���
	//      ֪ͨ����ظ�������״̬
	//
	void update(E_TEXTURE_TYPE type, YCITexture2D* texture);

	//
	// ������shrink(int poolId)
	//
	// Ŀ�ģ����������
	//
	// ע�ͣ�
	//      ��� poolId <> 0, ������Ӧ�������
	//      ��� poolId == 0, �������е������
	//
	void shrink(int poolId);

	// 
	// ����: purge(int poolId)
	//
	// Ŀ�ģ���������
	//
	// ע�ͣ�
	//      ��� poolId <> 0, ��ն�Ӧ�������
	//      ��� poolId == 0, ������е������
	//
	void purge(int poolId);

	//
	// ������dump()
	//
	// Ŀ�ģ�dump���ڲ�״̬ΪHtmlƬ��
	//
	virtual const char* dump();

	// 
	// ������run()
	//
	// Ŀ�ģ������߳�ʵ�ֺ���
	//
	// ע�ͣ� Ĭ�ϵȴ�myDurationSecondsʱ��
	//       �п��ܱ�shrink����purge����
	//
	virtual int run();

private:

	E_POOL_STATUS myStatus;				//�����״̬

	YCTextureFactory *myFactory;		//���������

	int myDurationSeconds;				// ����ʧЧʱ��

	YCTexturePoolImpl *myTexturePool;   // �����ʵ��
		
	YCThread* myLRUThread;				//����ؼ����߳�
	YCCondition* myLRUCondition;		//�̵߳ȴ�����

	scheduledItem myScheduledItem;		//������̱߳����Ѻ�Ĺ�����
};

#endif