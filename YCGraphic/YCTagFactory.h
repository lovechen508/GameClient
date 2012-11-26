#ifndef _INCLUDE_YCTAGFACTORY_H_
#define _INCLUDE_YCTAGFACTORY_H_

class YCDList;
class YCIUITag;
class YCConfig;
class YCTagBuilder;
class YCUITagMeta;

class YCTagFactory
{
public:

	YCTagFactory(void);

	~YCTagFactory(void);

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：初始化标签工厂
	//
	bool initialize(YCConfig& config);
		
	//
	// 函数：registerTag(YCUITagMeta* tag)
	//
	// 目的：注册标签
	//
	void registerTag(YCUITagMeta* tagMeta);

	//
	// 函数：create(const char* tagName)
	//
	// 目的：根据标签名产生标签
	//
	YCIUITag* create(const char* tagName);

	//
	// 函数：finalize()
	//
	// 目的：销毁YCTagFactory
	//
	void finalize();

private:

	YCTagBuilder *myTagBuilder;

	YCDList *myTagRepository;
};

#endif

