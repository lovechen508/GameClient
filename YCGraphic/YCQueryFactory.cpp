#include "YCQueryFactory.h"

#include "YCQuery.h"

#include "YCBasic\YCDef.h"

YCQueryFactory::YCQueryFactory(void)
	: myQuery(NULL)
{
}


YCQueryFactory::~YCQueryFactory(void)
{
	SAFE_DELETE(myQuery);
}

//
// ������createYCQuery()
//
// Ŀ�ģ�����YCQuery
//
YCIQuery* YCQueryFactory::createYCQuery(YCLuaContext* context)
{
	if (myQuery == NULL)
	{
		myQuery = new YCQuery();
	}

	myQuery->bind(context);
	return myQuery;
}