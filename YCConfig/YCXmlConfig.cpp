#include "StdAfx.h"
#include "YCXmlConfig.h"

using namespace tinyxml2;

#include <ctype.h>

static bool isInt(const char *tocheck)
{
    size_t length = strlen(tocheck);
    size_t loop = 0;

    while(loop < length)
    {
        if(!isdigit(tocheck[loop]))
        {
            return false;
        }
        loop++;
    }
	return true;
} 

YCXmlConfig::YCXmlConfig(const char *filename)
    : pos(NULL)
{
	doc = new XMLDocument();
    if (! doc->LoadFile(filename))
    {
        throw YCException(2002, doc->GetErrorStr1());
    }
    pos = doc->RootElement();
}

YCXmlConfig::~YCXmlConfig()
{
	if (NULL != doc)
	{
		delete doc;
	}
}

bool YCXmlConfig::validate()
{
    throw YCException(2002, "Not Implement yet!");
    return true;
}

YCXmlConfig& YCXmlConfig::enter(const char *node)
{
    if (NULL == node)
    {
        throw YCException(2002, "YCXmlConfig::enter node is null!");
    }
        
    if (NULL == pos || pos->NoChildren())
    {
        throw YCException(2002, "YCXmlConfig::enter pos is null!");
    }

    XMLNode *child = pos->FirstChildElement(node);
    if (NULL == child)
    {
        char info[200] = {0};
        sprintf_s(info,200, "YCXmlConfig::enter : Child %s not found!", node);
        throw YCException(2002, info);
    }
    pos = child;
	return *this;
}

YCXmlConfig& YCXmlConfig::value(std::string& val)
{
    if (NULL == pos)
    {
        throw YCException(2002, "YCXmlConfig::value invalid in-buf!");
    }
	val = get_value();
	return *this;
}

YCXmlConfig& YCXmlConfig::value(int &val)
{
	const char * text = get_value();
	if (!isInt(text))
	{
		throw YCException(2002, "YCXmlConfig::value not integer!");
	}
    val = atoi(text);
	return *this;
}

YCXmlConfig& YCXmlConfig::attr(const char *name, std::string& val)
{
	if (NULL == name)
    {
        throw YCException(2002, "YCXmlConfig::value name is null!");
    }

	if (NULL == pos)
    {
        throw YCException(2002, "YCXmlConfig::attr pos is null!");
    }

	XMLElement *e = pos->ToElement();
    if (NULL == e)
    {
        throw YCException(2002, "YCXmlConfig::attr invalid element!");
    }

	val = e->Attribute(name);
	return *this;
}

YCXmlConfig& YCXmlConfig::attr(const char *name, int& val)
{
	if (NULL == name)
    {
        throw YCException(2002, "YCXmlConfig::value name is null!");
    }

	if (NULL == pos)
    {
        throw YCException(2002, "YCXmlConfig::attr pos is null!");
    }

	XMLElement *e = pos->ToElement();
    if (NULL == e)
    {
        throw YCException(2002, "YCXmlConfig::attr invalid element!");
    }

	if (XML_SUCCESS != e->QueryIntAttribute(name, &val))
	{
		char info[200] = {0};
		sprintf_s(info, 200, "YCXmlConfig::attr %s failed", name);
		throw YCException(2002, info);
	}
	return *this;
}

YCXmlConfig& YCXmlConfig::leave()
{
	if (NULL == pos)
    {
        throw YCException(2002, "YCXmlConfig::leave pos is null!");
    }

    XMLNode *parent = pos->Parent();
    if (NULL == parent)
    {
        throw YCException(2002, "YCXmlConfig::leave Parent is null!");
    }
    pos = parent;
	return *this;
}

const char *YCXmlConfig::get_value()
{
	if (NULL == pos)
    {
        throw YCException(2002, "YCXmlConfig::get_value pos is null!");
    }

    XMLElement *e = pos->ToElement();
    if (NULL == e)
    {
        throw YCException(2002, "YCXmlConfig::get_value invalid element!");
    }
    const char *val = e->GetText();
    if (NULL == val)
    {
        throw YCException(2002, "YCXmlConfig::get_value has no value!");
    }
	return val;
}
