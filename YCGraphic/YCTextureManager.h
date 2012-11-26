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
 * YCTextureManager: 纹理管理类
 *
 * 注释：完成纹理在内存中管理，淘汰算法
 *       而纹理的下载，加载则交由YCTextureFactory类完成
 *       整个纹理管理系统，对外只暴露YCTextureManager和YCTexture
 *       对客户端而言，YCTexture YCTextureManager::fetch(E_TEXTURE_TYPE type, const char* filename) 就是全部操作
 *
 *           ----------------     TexturePool desc
 *          |                |    
 *          |                | ->     1       TileMap  ->【YCITexture2D*】->【YCITexture2D*】->【YCITexture2D*】-> ...
 *          |                |
 *          |                | ->     2       SmallMap ->【YCITexture2D*】->【YCITexture2D*】->【YCITexture2D*】-> ...
 *          |                |
 *          |                | ->     3       Obstacle ->【YCITexture2D*】->【YCITexture2D*】->【YCITexture2D*】-> ...
 *  acquire |                |
 *  <-------|                | ->     4       NPC      ->【YCITexture2D*】->【YCITexture2D*】->【YCITexture2D*】-> ...
 *          |                |
 *          |YCTextureManager| ->     5       Player   ->【YCITexture2D*】->【YCITexture2D*】->【YCITexture2D*】-> ...
 *  release |                | 
 *  ------->|                | ->     6       Monster  ->【YCITexture2D*】->【YCITexture2D*】->【YCITexture2D*】-> ...
 *          |                |
 *          |                | ->     7       Effect   ->【YCITexture2D*】->【YCITexture2D*】->【YCITexture2D*】-> ...
 *          |                |
 *          |                | ->     8       UI       ->【YCITexture2D*】->【YCITexture2D*】->【YCITexture2D*】-> ...
 *          |                |
 *          |                | ->     9       ...
 *          |                |
 *           ----------------
 *
 *  纹理在文件目录树中的分布：
 *
 *   根目录    类别              TextureID
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
		int actionType;   //0: 例行, 1: shrink, 2: purge
		int parameter;    // 纹理池编号, 0表示全部
	};

	typedef enum PoolStatus{STOPPED = 0, STARTED = 1} E_POOL_STATUS;

public:

	YCTextureManager();

	~YCTextureManager();

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
	// 目的：释放Http网络连接池   
	//       停止LRU监视线程  
	//       释放内部数据结构
	//
	void finalize();

	//
	// 函数：fetch(E_TEXTURE_TYPE type, const chra* filename)
	//
	// 目的：获取纹理
	//      客户唯一访问纹理接口
	//
	// 参数：
	//		filename : 相对路径名
	//
	// 注释：
	//      客户必须保证纹理类型的正确和uniqueId的唯一性，用于本地缓存查找
	//      如果本地缓存命中，直接返回对应纹理
	//      否则，请求YCTextureFactory获取纹理
	//		    YCTextureFactory:   如果对应目录文件尚未下载，返回YCTexture2DFuture,同时向YCINetworkLoader提交下载请求
	//                              如果对应目录下文件存在，读入并构建YCSingleTexture2D
	//                              如果对应目录下存在TextureManager生成的XML Altas文件，读入并构建YCAtlasTexture2D
	//
	YCTexture* fetch(E_TEXTURE_TYPE type, const char* filename);

	//
	// 函数：update(E_TEXTURE_TYPE type, YCITexture2D* texture);
	//
	// 目的：用于YCTextureFactory下载，加载纹理成功后
	//      通知纹理池更新纹理状态
	//
	void update(E_TEXTURE_TYPE type, YCITexture2D* texture);

	//
	// 函数：shrink(int poolId)
	//
	// 目的：收缩纹理池
	//
	// 注释：
	//      如果 poolId <> 0, 收缩对应的纹理池
	//      如果 poolId == 0, 收缩所有的纹理池
	//
	void shrink(int poolId);

	// 
	// 函数: purge(int poolId)
	//
	// 目的：清空纹理池
	//
	// 注释：
	//      如果 poolId <> 0, 清空对应的纹理池
	//      如果 poolId == 0, 清空所有的纹理池
	//
	void purge(int poolId);

	//
	// 函数：dump()
	//
	// 目的：dump类内部状态为Html片段
	//
	virtual const char* dump();

	// 
	// 函数：run()
	//
	// 目的：监视线程实现函数
	//
	// 注释： 默认等待myDurationSeconds时间
	//       有可能被shrink，或purge唤醒
	//
	virtual int run();

private:

	E_POOL_STATUS myStatus;				//纹理池状态

	YCTextureFactory *myFactory;		//纹理加载器

	int myDurationSeconds;				// 纹理失效时间

	YCTexturePoolImpl *myTexturePool;   // 纹理池实现
		
	YCThread* myLRUThread;				//纹理池监视线程
	YCCondition* myLRUCondition;		//线程等待条件

	scheduledItem myScheduledItem;		//纹理池线程被唤醒后的工作项
};

#endif