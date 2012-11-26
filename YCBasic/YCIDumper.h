#ifndef _INCLUDE_YCIDUMPER_H_
#define _INCLUDE_YCIDUMPER_H_

/*
 * YCIDumper : Dump 类内部状态到html
 */
class YCAPI YCIDumper
{
public:

	YCIDumper() {}

	virtual ~YCIDumper() {}

	//
	// 函数：dump()
	//
	// 目的：dump类内部状态为Html片段
	//
	virtual const char* dump() = 0;
};
#endif