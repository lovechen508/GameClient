#ifndef _INCLUDE_YCMAGICMETAMANAGER_H_
#define _INCLUDE_YCMAGICMETAMANAGER_H_

#include <map>

class YCMagicMeta;

/*
 * YCMagicMetaManager : 技能元数据管理类
 */
class YCMagicMetaManager 
{
public:

	YCMagicMetaManager(void);
	
	~YCMagicMetaManager(void);

	//
	// 函数：setMetaInfo(YCMagicMeta* magicMeta)
	//
	// 目的：增加技能元数据对象
	//
	void setMetaInfo(YCMagicMeta* magicMeta);
	//
	// 函数：getMetaInfo(unsigned int magicId)
	//
	// 目的：获取技能元数据对象
	//
	YCMagicMeta* getMetaInfo(unsigned int magicId);

private:

	//
	// 技能元数据映射
	//
	std::map<unsigned int, YCMagicMeta*> myMagicMetas; 
};

#endif

