#include "YCUIDomBuilder.h"

#include <memory>

#include "YCUIDOM.h"
#include "YCIContainer.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"
#include "YCConfig\tinyxml2.h"

//
// ������handleElement(tinyxml2::XMLElement* element, YCUIDOM* parent)
//
// Ŀ�ģ��ݹ鴦��YCUIDOM�ڲ�����
//
YCUIDOM* handleElement(const tinyxml2::XMLElement* element, YCUIDOM* parent)
{
	if (element == NULL)
	{
		throw YCException(2002, "YCUIDomBuilder::handleElement����ΪNULL");
	}

	YCUIDOM* current = NULL;
	const char* name = element->Name();
	if (strcmp(name, "script") == 0)
	{
		if (parent == NULL)
		{
			throw YCException(2002, "YCUIDomBuilder::handleElement�ĵ��쳣��script�����ڶ���ڵ���!");
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
			throw YCException(2002, "YCUIDomBuilder::handleElement�ĵ��쳣��layout�����ڶ���ڵ���!");
		}
		const tinyxml2::XMLAttribute * attr = element->FindAttribute("file");
		if (attr != NULL)
		{
			parent->setLayout(attr->Value());
		}
	}
	else
	{
		// ���ñ�ǩ��,ֵ
		current = new YCUIDOM();
		current->setName(element->Name());
		current->setValue(element->GetText());
		current->setParent(parent);

		// ���ñ�ǩ����
		for (const tinyxml2::XMLAttribute* attr = element->FirstAttribute();
			 attr != NULL;
			 attr = attr->Next())
		{
			current->setAttribute(attr->Name(), attr->Value());
		}

		// �����ӱ�ǩ
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
				LOG_WARNING("YCUIDomBuilder::handleElement����ڵ��쳣��" << e.what());
			}
		}
	}

	return current;
}

//
// ������build(const char* xmlFile)
//
// Ŀ�ģ���UIXml�����ļ��д���DOM
//
YCUIDOM* YCUIDomBuilder::build(const char* xmlFile)
{
	if (xmlFile == NULL)
	{
		throw YCException(2002, "YCUIDomBuilder::build��xmlFileΪNULL");
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
		LOG_WARNING("YCUIDomBuilder::build��Ч�ļ���������" << tagName);
		throw YCException(2002, "YCUIDomBuilder::build��Ч�ļ�����");
	}
	
	YCUIDOM* uiDom = handleElement(containerTag, NULL);
	if (uiDom != NULL)
	{
		uiDom->setFilename(xmlFile);
	}
	return uiDom;
}

//
// ������Parse(const char* strValue);
//
// Ŀ�ģ���strValue�ַ����д���DOM
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