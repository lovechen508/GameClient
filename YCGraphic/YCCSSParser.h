#ifndef _INCLUDE_YCCSSPARSER_H_
#define _INCLUDE_YCCSSPARSER_H_

class YCCSSContext;

/*
 * YCCSSParser : CSS�ļ������������࣬���ڲ�״̬
 */
struct YCCSSParser
{
	//
	// ������parse(YCCSSContext* context, const char * filename)
	//
	// Ŀ�ģ�����CSS�ļ������뵽YCCSSContext��
	//
	static bool parse(YCCSSContext* context, const char * filename);
};

#endif