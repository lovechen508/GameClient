#ifndef _INCLUDE_YCMAGICBUILDER_H_
#define _INCLUDE_YCMAGICBUILDER_H_

class YCMagic;
struct YCIPackageResponse;

/*
 * YCMagicBuilder : 技能构建辅助类
 */
struct YCMagicBuilder
{
	//
	// 函数：build(YCIPackageResponse* response)
	//
	// 目的：从服务器返回构建技能
	//
	static YCMagic* build(YCIPackageResponse* response);
};

#endif

