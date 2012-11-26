#ifndef _INCLUDE_YCQUERYFACTORY_H_
#define _INCLUDE_YCQUERYFACTORY_H_

#include "YCLua\YCIQueryFactory.h"

class YCQuery;

class YCQueryFactory :
	public YCIQueryFactory
{
public:

	YCQueryFactory(void);

	virtual ~YCQueryFactory(void);

	//
	// 函数：createYCQuery()
	//
	// 目的：创建YCQuery
	//
	virtual YCIQuery* createYCQuery(YCLuaContext* context);

private:

	YCQuery * myQuery;
};

#endif