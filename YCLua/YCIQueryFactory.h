#ifndef _INCLUDE_YCIQUERYFACTORY_H_
#define _INCLUDE_YCIQUERYFACTORY_H_

class YCIQuery;
class YCLuaContext;

/*
 * YCIQueryFactory : 工厂类
 */
class YCIQueryFactory
{
public:

	YCIQueryFactory() {}

	virtual ~YCIQueryFactory() {}

	//
	// 函数：createYCQuery()
	//
	// 目的：创建YCQuery
	//
	virtual YCIQuery* createYCQuery(YCLuaContext* context) = 0;

};

#endif
