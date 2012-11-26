#include "YCTagBuilder.h"


#include "YCTagFactory.h"
#include "YCUITagMeta.h"
#include "YCUITagHelper.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCConfig\YCConfig.h"
#include "YCConfig\YCXmlReader.h"

#pragma region coreTagPath

static void* handle(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(customData != NULL);
		
	const char *fileExt = YCFileUtil::GetFileExt(filename);
	if (fileExt == NULL)
	{
		LOG_WARNING("YCTagBuilder::notify未知文件名：" << filename);
		return NULL;
	}

	if (strcmp(fileExt, "def") == 0)
	{
		YCTagBuilder* tagBuilder = (YCTagBuilder*)customData;
		try 
		{
			char theFullPath[MAX_PATH] ={0};
			sprintf_s(theFullPath, "%s\\%s", path, filename);
			tagBuilder->build(theFullPath);
		}
		catch (YCException& e)
		{
			LOG_WARNING("YCTagBuilder::build出错：" << e.what());
		}
	}
	else
	{
		LOG_WARNING("YCTagBuilder::build未知文件类型：" << fileExt);
	}

	return NULL;
}

#pragma endregion coreTagPath

YCTagBuilder::YCTagBuilder(void)
	: myTagFactory(NULL)
{
}


YCTagBuilder::~YCTagBuilder(void)
{
}

//
// 函数：initialize(YCTagFactory* factory, YCConfig& config)
//
// 目的：初始化标签制造器
//
bool YCTagBuilder::initialize(YCTagFactory* factory, YCConfig& config)
{
	if (factory == NULL)
	{
		throw YCException(2002, "YCTagBuilder::initialize中factory为空！");
	}
	myTagFactory = factory;

	const char * coreTagPath = config.coreTagPath();
	if (coreTagPath == NULL)
	{
		throw YCException(2002, "YCTagBuilder::initialize中coreTagPath为空！");
	}

	//获取当前工作目录
	char theExePath[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(theExePath, MAX_PATH))
	{
		return false;
	}

	if ( (strlen(coreTagPath) + strlen(theExePath)) >= MAX_PATH )
	{
		throw YCException(2002, "YCTagBuilder::initialize中core标签目录过长！");
	}

	char theFullPath[MAX_PATH] = {0};
	sprintf_s(theFullPath, "%s\\..\\%s", theExePath, coreTagPath);
	LOG_INFO("YCTagBuilder::initialize中核心标签路径：" << theFullPath);
	if (YCFileUtil::IsFileExist(theFullPath))
	{
		std::string pathWithDefFilter(theFullPath);
		pathWithDefFilter += "\\*.def";
		YCFileUtil::HandleFileInDir(pathWithDefFilter.c_str(), handle, this); 
	}

	return true;
}

//
// 函数：build(const char* tagfile)
//
// 目的：从标签文件中创建标签
//
YCUITagMeta* YCTagBuilder::build(const char* tagfile)
{
	LOG_DEBUG("YCTagBuilder::build处理：" << tagfile);

	YCXmlReader reader(tagfile);

	if (!reader.validate())
	{
		throw YCException(2002, "YCTagBuilder::build处理Tag文件格式错误");
	}

	char * name;
	char * baseTagName;

	reader.attr("name", &name)
		  .attr("baseTag", &baseTagName);
	YCUITagMeta * tagMeta = new YCUITagMeta(name);
	if (strcmp(baseTagName, "none") == 0)
	{
		SAFE_DELETE(baseTagName);
	}
	else
	{
		tagMeta->setBaseTagName(baseTagName);
	}

	reader.foreach("attr", YCXmlReader::YCForEachWrapper1<YCTagBuilder, YCUITagMeta>(this, &YCTagBuilder::handleAttribute, tagMeta));
	reader.foreach("css", YCXmlReader::YCForEachWrapper1<YCTagBuilder, YCUITagMeta>(this, &YCTagBuilder::handleCSSAttribute, tagMeta));
	myTagFactory->registerTag(tagMeta);
	return tagMeta;
}

/////////////////////////////////////////////////////////////////////////////
// Private Part
/////////////////////////////////////////////////////////////////////////////

//
// 函数：handleAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
//
// 目的：依次处理各个预定义属性
// 
bool YCTagBuilder::handleAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
{
	bool result = false;
	char * attribute = NULL;

	try {
		reader.attr("name", &attribute);
		metaTag->enableAttribute(attribute);
		result = true;
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCTagBuilder::handleAttribute处理属性名称出错" << e.what());
	}

	delete attribute;
	return true;
}

//
// 函数：handleCSSAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
//
// 目的：依次处理各个预定义CSS属性
// 
bool YCTagBuilder::handleCSSAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
{
	bool result = false;
	char * attribute = NULL;

	try {
		reader.attr("name", &attribute);
		metaTag->enableCSS(YCUITagHelper::Instance().getPropertyEnum(attribute));
		result = true;
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCTagBuilder::handleAttribute处理属性名称出错" << e.what());
	}

	delete attribute;
	return true;
}