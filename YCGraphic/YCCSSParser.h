#ifndef _INCLUDE_YCCSSPARSER_H_
#define _INCLUDE_YCCSSPARSER_H_

class YCCSSContext;

/*
 * YCCSSParser : CSS文件解释器工具类，无内部状态
 */
struct YCCSSParser
{
	//
	// 函数：parse(YCCSSContext* context, const char * filename)
	//
	// 目的：解析CSS文件，加入到YCCSSContext中
	//
	static bool parse(YCCSSContext* context, const char * filename);
};

#endif