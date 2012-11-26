#include "YCUIDomBuilder.h"

#include <memory>

#include "YCUIDOM.h"
#include "YCIContainer.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"
#include "YCConfig\tinyxml2.h"

//
// 函数：handleElement(tinyxml2::XMLElement* element, YCUIDOM* parent)
//
// 目的：递归处理YCUIDOM内部函数
//
YCUIDOM* handleElement(const tinyxml2::XMLElement* element, YCUIDOM* parent)
{
	if (element == NULL)
	{
		throw YCException(2002, "YCUIDomBuilder::handleElement参数为NULL");
	}

	YCUIDOM* current = NULL;
	const char* name = element->Name();
	if (strcmp(name, "script") == 0)
	{
		if (parent == NULL)
		{
			throw YCException(2002, "YCUIDomBuilder::handleElement文档异常：script出现在顶层节点中!");
		}
		const tinyxml2::XMLAttribute * attr = element->FindAttribute("file");
		if (attr != NULL)
		{
			parent->setScript(attr->Value());
		}
	}
	else if (strcmp(name, "layout") == 0)
	{
		if (parent == NULL)
		{
			throw YCException(2002, "YCUIDomBuilder::handleElement文档异常：layout出现在顶层节点中!");
		}
		const tinyxml2::XMLAttribute * attr = element->FindAttribute("file");
		if (attr != NULL)
		{
			parent->setLayout(attr->Value());
		}
	}
	else
	{
		// 设置标签名,值
		current = new YCUIDOM();
		current->setName(element->Name());
		current->setValue(element->GetText());
		current->setParent(parent);

		// 设置标签属性
		for (const tinyxml2::XMLAttribute* attr = element->FirstAttribute();
			 attr != NULL;
			 attr = attr->Next())
		{
			current->setAttribute(attr->Name(), attr->Value());
		}

		// 处理子标签
		for (const tinyxml2::XMLElement * subElement = element->FirstChildElement();
			 subElement != NULL;
			 subElement = subElement->NextSiblingElement())
		{		
			try
			{
				current->addSubDom(handleElement(subElement, current));
			}
			catch (YCException& e)
			{
				LOG_WARNING("YCUIDomBuilder::handleElement处理节点异常：" << e.what());
			}
		}
	}

	return current;
}

//
// 函数：build(const char* xmlFile)
//
// 目的：从UIXml配置文件中创建DOM
//
YCUIDOM* YCUIDomBuilder::build(const char* xmlFile)
{
	if (xmlFile == NULL)
	{
		throw YCException(2002, "YCUIDomBuilder::build中xmlFile为NULL");
	}

	std::auto_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument());
	if (tinyxml2::XML_NO_ERROR != doc->LoadFile(xmlFile))
    {
        throw YCException(2002, doc->GetErrorStr1());
    }

	tinyxml2::XMLElement* rootElement = doc->RootElement();  //components
	tinyxml2::XMLElement* containerTag = rootElement->FirstChildElement();  //window or dialog

	const char* tagName = containerTag->Name();
	if ( (strcmp(tagName, "window") != 0)
	   &&(strcmp(tagName, "dialog") != 0) )  
	{
		LOG_WARNING("YCUIDomBuilder::build无效文件配置名：" << tagName);
		throw YCException(2002, "YCUIDomBuilder::build无效文件配置");
	}
	
	YCUIDOM* uiDom = handleElement(containerTag, NULL);
	if (uiDom != NULL)
	{
		uiDom->setFilename(xmlFile);
	}
	return uiDom;
}

//
// 函数：Parse(const char* strValue);
//
// 目的：从strValue字符串中创建DOM
//
YCUIDOM* YCUIDomBuilder::Parse(const char* strValue)
{
	if (strValue == 0)
	{
		return NULL;
	}

	std::auto_ptr<tinyxml2::XMLDocument> doc(new tinyxml2::XMLDocument());
	if (tinyxml2::XML_NO_ERROR != doc->Parse(strValue))
	{
		throw YCException(2002, doc->GetErrorStr1());
	}

	return handleElement(doc->RootElement(), NULL);
}