#ifndef _INCLUDE_YCMAGICBUILDER_H_
#define _INCLUDE_YCMAGICBUILDER_H_

class YCMagic;

/*
 * YCMagicBuilder : 技能构建辅助类
 */
struct YCMagicBuilder
{
	//
	// 函数：build(void* response)
	//
	// 目的：从服务器返回构建技能
	//
	static YCMagic* build(void* response);
};

#endif

