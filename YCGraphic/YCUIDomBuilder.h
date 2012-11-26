#ifndef _INCLUDE_YCUIDOMBUILDER_H_
#define _INCLUDE_YCUIDOMBUILDER_H_

class YCUIDOM;

/*
 * YCUIDomBuilder : UI 窗口构建器
 */

class YCUIDomBuilder 
{

public:

	//
	// 函数：build(const char* xmlFile)
	//
	// 目的：从UIXml配置文件中创建DOM
	//
	static YCUIDOM* build(const char* xmlFile);

	//
	// 函数：Parse(const char* strValue);
	//
	// 目的：从strValue字符串中创建DOM
	//
	static YCUIDOM* Parse(const char* strValue);
};

#endif