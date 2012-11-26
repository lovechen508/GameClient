#ifndef _INCLUDE_YCMAGICMETABUILDER_H_
#define _INCLUDE_YCMAGICMETABUILDER_H_

class YCMagicMetaManager;

/*
 * YCMagicMetaBuilder : 技能元数据构建器
 */
struct YCMagicMetaBuilder
{
	//
	// 函数：build(const char* filename, YCMagicMetaManager* manager)
	//
	// 目的：根据配置文件创建技能元数据
	//
	bool build(const char* filename, YCMagicMetaManager* manager);
};

#endif

