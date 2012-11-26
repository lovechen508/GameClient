#ifndef _INCLUDE_YCTAGBUILDER_H_
#define _INCLUDE_YCTAGBUILDER_H_

class YCConfig;
class YCUITagMeta;
class YCTagFactory;
class YCXmlReader;

/*
 * YCTagBuilder : 标签构造器
 */
class YCTagBuilder
{

public:

	YCTagBuilder(void);
	
	~YCTagBuilder(void);

	//
	// 函数：initialize(YCTagFactory* factory, YCConfig& config)
	//
	// 目的：初始化标签制造器
	//
	bool initialize(YCTagFactory* factory, YCConfig& config);

	//
	// 函数：build(const char* tagfile)
	//
	// 目的：从标签文件中创建标签
	//
	YCUITagMeta* build(const char* tagfile);

private:

	//
	// 函数：handleAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
	//
	// 目的：依次处理各个预定义属性
	// 
	bool handleAttribute(YCXmlReader& reader, YCUITagMeta *metaTag);

	//
	// 函数：handleCSSAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
	//
	// 目的：依次处理各个预定义CSS属性
	// 
	bool handleCSSAttribute(YCXmlReader& reader, YCUITagMeta *metaTag);

private:

	YCTagFactory * myTagFactory;

};

#endif

