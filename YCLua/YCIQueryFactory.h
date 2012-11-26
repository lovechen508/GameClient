#ifndef _INCLUDE_YCIQUERYFACTORY_H_
#define _INCLUDE_YCIQUERYFACTORY_H_

class YCIQuery;
class YCLuaContext;

/*
 * YCIQueryFactory : ������
 */
class YCIQueryFactory
{
public:

	YCIQueryFactory() {}

	virtual ~YCIQueryFactory() {}

	//
	// ������createYCQuery()
	//
	// Ŀ�ģ�����YCQuery
	//
	virtual YCIQuery* createYCQuery(YCLuaContext* context) = 0;

};

#endif
