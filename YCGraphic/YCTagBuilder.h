#ifndef _INCLUDE_YCTAGBUILDER_H_
#define _INCLUDE_YCTAGBUILDER_H_

class YCConfig;
class YCUITagMeta;
class YCTagFactory;
class YCXmlReader;

/*
 * YCTagBuilder : ��ǩ������
 */
class YCTagBuilder
{

public:

	YCTagBuilder(void);
	
	~YCTagBuilder(void);

	//
	// ������initialize(YCTagFactory* factory, YCConfig& config)
	//
	// Ŀ�ģ���ʼ����ǩ������
	//
	bool initialize(YCTagFactory* factory, YCConfig& config);

	//
	// ������build(const char* tagfile)
	//
	// Ŀ�ģ��ӱ�ǩ�ļ��д�����ǩ
	//
	YCUITagMeta* build(const char* tagfile);

private:

	//
	// ������handleAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
	//
	// Ŀ�ģ����δ������Ԥ��������
	// 
	bool handleAttribute(YCXmlReader& reader, YCUITagMeta *metaTag);

	//
	// ������handleCSSAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
	//
	// Ŀ�ģ����δ������Ԥ����CSS����
	// 
	bool handleCSSAttribute(YCXmlReader& reader, YCUITagMeta *metaTag);

private:

	YCTagFactory * myTagFactory;

};

#endif

