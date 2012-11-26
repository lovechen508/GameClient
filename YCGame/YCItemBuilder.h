#ifndef _INCLUDE_YCITEMBUILDER_H_
#define _INCLUDE_YCITEMBUILDER_H_

class YCItem;
struct YCIPackageResponse;

struct YCItemBuilder
{
	//
	// 函数：build(YCIPackageResponse* response)
	//
	// 目的：从服务器返回构建物品
	//
	static YCItem* build(YCIPackageResponse* response);
};

#endif