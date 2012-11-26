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
		LOG_WARNING("YCTagBuilder::notifyδ֪�ļ�����" << filename);
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
			LOG_WARNING("YCTagBuilder::build����" << e.what());
		}
	}
	else
	{
		LOG_WARNING("YCTagBuilder::buildδ֪�ļ����ͣ�" << fileExt);
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
// ������initialize(YCTagFactory* factory, YCConfig& config)
//
// Ŀ�ģ���ʼ����ǩ������
//
bool YCTagBuilder::initialize(YCTagFactory* factory, YCConfig& config)
{
	if (factory == NULL)
	{
		throw YCException(2002, "YCTagBuilder::initialize��factoryΪ�գ�");
	}
	myTagFactory = factory;

	const char * coreTagPath = config.coreTagPath();
	if (coreTagPath == NULL)
	{
		throw YCException(2002, "YCTagBuilder::initialize��coreTagPathΪ�գ�");
	}

	//��ȡ��ǰ����Ŀ¼
	char theExePath[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(theExePath, MAX_PATH))
	{
		return false;
	}

	if ( (strlen(coreTagPath) + strlen(theExePath)) >= MAX_PATH )
	{
		throw YCException(2002, "YCTagBuilder::initialize��core��ǩĿ¼������");
	}

	char theFullPath[MAX_PATH] = {0};
	sprintf_s(theFullPath, "%s\\..\\%s", theExePath, coreTagPath);
	LOG_INFO("YCTagBuilder::initialize�к��ı�ǩ·����" << theFullPath);
	if (YCFileUtil::IsFileExist(theFullPath))
	{
		std::string pathWithDefFilter(theFullPath);
		pathWithDefFilter += "\\*.def";
		YCFileUtil::HandleFileInDir(pathWithDefFilter.c_str(), handle, this); 
	}

	return true;
}

//
// ������build(const char* tagfile)
//
// Ŀ�ģ��ӱ�ǩ�ļ��д�����ǩ
//
YCUITagMeta* YCTagBuilder::build(const char* tagfile)
{
	LOG_DEBUG("YCTagBuilder::build����" << tagfile);

	YCXmlReader reader(tagfile);

	if (!reader.validate())
	{
		throw YCException(2002, "YCTagBuilder::build����Tag�ļ���ʽ����");
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
// ������handleAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
//
// Ŀ�ģ����δ������Ԥ��������
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
		LOG_WARNING("YCTagBuilder::handleAttribute�����������Ƴ���" << e.what());
	}

	delete attribute;
	return true;
}

//
// ������handleCSSAttribute(YCXmlReader& reader, YCUITagMeta *metaTag)
//
// Ŀ�ģ����δ������Ԥ����CSS����
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
		LOG_WARNING("YCTagBuilder::handleAttribute�����������Ƴ���" << e.what());
	}

	delete attribute;
	return true;
}