#include "YCXmlReader.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCException.h"

#include "tinyxml2.h"

#include <ctype.h>

//
// 函数：isInt(const char *tocheck)
//
// 目的：值是否为integer
//
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

/*
 * YCXmlReaderImpl : 内部辅助类
 *  
 * 目的: 对第三方的简单包装
 *       对客户完全屏蔽第三方xml parser
 */
class YCXmlReaderImpl
{
public:

	YCXmlReaderImpl() 
		: doc(NULL)
		, pos(NULL) 
	{
	}

	~YCXmlReaderImpl()
	{
		delete doc;
	}

	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLNode *pos;
};

//
// 函数：YCXmlReader(const char *file)
//
// 目的：构造函数，从文件创建
//
YCXmlReader::YCXmlReader(const char *filename)
    : /*file(filename), */myReaderImpl(new YCXmlReaderImpl())
{
	myReaderImpl->doc = new tinyxml2::XMLDocument();
    if (tinyxml2::XML_NO_ERROR != myReaderImpl->doc->LoadFile(filename))
    {
        throw YCException(2002, myReaderImpl->doc->GetErrorStr1());
    }
    myReaderImpl->pos = myReaderImpl->doc->RootElement();
}

//
// 函数：YCXmlReader(const char* buffer, unsigned int length)
//
// 目的：构造函数，从内存段创建
//
YCXmlReader::YCXmlReader(const char* buffer, unsigned int length)
	: myReaderImpl(new YCXmlReaderImpl())
{
	SMART_ASSERT(buffer != NULL);
	SMART_ASSERT(length > 0);

	

	myReaderImpl->doc = new tinyxml2::XMLDocument();
	if (tinyxml2::XML_NO_ERROR != myReaderImpl->doc->Parse(buffer))
	{
		throw YCException(2002, myReaderImpl->doc->GetErrorStr1());
	}
	myReaderImpl->pos = myReaderImpl->doc->RootElement();
}

YCXmlReader::~YCXmlReader()
{
	SAFE_DELETE(myReaderImpl);
}

//
// 函数：validate()
//
// 目的：校验XML文档的完整性
//
bool YCXmlReader::validate()
{
   // throw YCException(2002, "Not Implement yet!");
    return true;
}

// 
// 函数：enter(const char* node)
//
// 目的：进入子节点
//
YCXmlReader& YCXmlReader::enter(const char *node)
{
    if (NULL == node)
    {
        throw YCException(2002, "YCXmlReader::enter node is null!");
    }
        
    if (NULL == myReaderImpl->pos || myReaderImpl->pos->NoChildren())
    {
        throw YCException(2002, "YCXmlReader::enter pos is null!");
    }

    tinyxml2::XMLNode *child = myReaderImpl->pos->FirstChildElement(node);
    if (NULL == child)
    {
        char info[200] = {0};
        sprintf_s(info,200, "YCXmlReader::enter : Child %s not found!", node);
        throw YCException(2002, info);
    }
    myReaderImpl->pos = child;
	return *this;
}

//
// 函数：value(char** val)
//
// 目的：取字符串值，val的内存在里面分配
//
YCXmlReader& YCXmlReader::value(char** val)
{
    if (NULL == myReaderImpl->pos)
    {
        throw YCException(2002, "YCXmlReader::value invalid in-buf!");
    }

	const char* tmp = get_value();
	int length = strlen(tmp);
	*val = new char[length+1];
	strncpy(*val, tmp, length);
	(*val)[length] = '\0';

	return *this;
}

//
// 函数：value(char** val, const char* defaultValue)
//
// 目的：取字符串值，val的内存在里面分配
//
YCXmlReader& YCXmlReader::value(char** val, const char* defaultValue)
{
	try 
	{
		this->value(val);
	}
	catch (YCException &e)
	{
		UNREFERENCED_PARAMETER(e);
		int length = strlen(defaultValue);
		*val = new char[length+1];
		strncpy(*val, defaultValue, length);
		(*val)[length] = '\0';
	}
	return *this;
}

//
// 函数：value(int& val)
//
// 目的：取int型值
//
YCXmlReader& YCXmlReader::value(int &val)
{
	const char * text = get_value();
	if (!isInt(text))
	{
		throw YCException(2002, "YCXmlReader::value not integer!");
	}
    val = atoi(text);
	return *this;
}

//
// 函数：value(int& val, int defaultValue)
//
// 目的：取int型值
//
YCXmlReader& YCXmlReader::value(int& val, int defaultValue)
{
	try 
	{
		this->value(val);
	}
	catch (YCException& e)
	{
		UNREFERENCED_PARAMETER(e);
		val = defaultValue;
	}
	return *this;
}

//
// 函数：value(unsigned int& val)
//
// 目的：取unsigned int型值
//
YCXmlReader& YCXmlReader::value(unsigned int& val)
{
	const char * text = get_value();
	if (!isInt(text))
	{
		throw YCException(2002, "YCXmlReader::value not integer!");
	}
	val = atol(text);
	return *this;
}

//
// 函数：value(unsigned int& val, unsigned int defaultValue)
//
// 目的：取int型值
//
YCXmlReader& YCXmlReader::value(unsigned int& val, unsigned int defaultValue)
{
	try 
	{
		this->value(val);
	}
	catch (YCException& e)
	{
		UNREFERENCED_PARAMETER(e);
		val = defaultValue;
	}
	return *this;
}

//
// 函数：attr(const char *name, char** str)
//
// 目的：取字符串属性值，val的内存在里面分配
//
YCXmlReader& YCXmlReader::attr(const char *name, char** val)
{
	if (NULL == name)
    {
        throw YCException(2002, "YCXmlReader::value name is null!");
    }

	if (NULL == myReaderImpl->pos)
    {
        throw YCException(2002, "YCXmlReader::attr pos is null!");
    }

	tinyxml2::XMLElement *e = myReaderImpl->pos->ToElement();
    if (NULL == e)
    {
        throw YCException(2002, "YCXmlReader::attr invalid element!");
    }

	const char* tmp = e->Attribute(name);
	int length = strlen(tmp);
	*val = new char[length+1];
	strncpy(*val, tmp, length);
	(*val)[length] = '\0';
	
	return *this;
}

//
// 函数：attr(const char *name, char** str, const char* defaultValue)
//
// 目的：取字符串属性值，val的内存在里面分配
//
YCXmlReader& YCXmlReader::attr(const char *name, char** val, const char* defaultValue)
{
	try
	{
		this->attr(name, val);
	}
	catch (YCException& e)
	{
		UNREFERENCED_PARAMETER(e);
		int length = strlen(defaultValue);
		*val = new char[length+1];
		strncpy(*val, defaultValue, length);
		(*val)[length] = '\0';
	}
	return *this;
}

//
// 函数：attr(const char *name, int& str)
//
// 目的：取int型属性值
//
YCXmlReader& YCXmlReader::attr(const char *name, int& val)
{
	if (NULL == name)
    {
        throw YCException(2002, "YCXmlReader::value name is null!");
    }

	if (NULL == myReaderImpl->pos)
    {
        throw YCException(2002, "YCXmlReader::attr pos is null!");
    }

	tinyxml2::XMLElement *e = myReaderImpl->pos->ToElement();
    if (NULL == e)
    {
        throw YCException(2002, "YCXmlReader::attr invalid element!");
    }

	if (tinyxml2::XML_SUCCESS != e->QueryIntAttribute(name, &val))
	{
		char info[200] = {0};
		sprintf_s(info, 200, "YCXmlReader::attr %s failed", name);
		throw YCException(2002, info);
	}
	return *this;
}

//
// 函数：attr(const char *name, int& str, int defaultValue)
//
// 目的：取int型属性值
//
YCXmlReader& YCXmlReader::attr(const char *name, int& val, int defaultValue)
{
	try
	{
		this->attr(name, val);
	}
	catch (YCException& e)
	{
		UNREFERENCED_PARAMETER(e);
		val = defaultValue;
	}
	return *this;
}

//
// 函数：attr(const char *name, unsigned int& str)
//
// 目的：取int型属性值
//
YCXmlReader& YCXmlReader::attr(const char *name, unsigned int& val)
{
	if (NULL == name)
	{
		throw YCException(2002, "YCXmlReader::value name is null!");
	}

	if (NULL == myReaderImpl->pos)
	{
		throw YCException(2002, "YCXmlReader::attr pos is null!");
	}

	tinyxml2::XMLElement *e = myReaderImpl->pos->ToElement();
	if (NULL == e)
	{
		throw YCException(2002, "YCXmlReader::attr invalid element!");
	}

	if (tinyxml2::XML_SUCCESS != e->QueryUnsignedAttribute(name, &val))
	{
		char info[200] = {0};
		sprintf_s(info, 200, "YCXmlReader::attr %s failed", name);
		throw YCException(2002, info);
	}
	return *this;
}

//
// 函数：attr(const char *name, int& str, unsigned int defaultValue)
//
// 目的：取int型属性值
//
YCXmlReader& YCXmlReader::attr(const char *name, unsigned int& val, unsigned int defaultValue)
{
	try
	{
		this->attr(name, val);
	}
	catch (YCException& e)
	{
		UNREFERENCED_PARAMETER(e);
		val = defaultValue;
	}
	return *this;
}

//
// 函数：foreach(const char *node, YCIForEachWrapper& wrapper)
//
// 目的：遍历名为node的子节点
//
YCXmlReader& YCXmlReader::foreach(const char *node, YCIForEachWrapper& wrapper)
{
	if (NULL == node)
	{
		throw YCException(2002, "YCXmlReader::foreach invalid in-params!");
	}

	if (NULL == myReaderImpl->pos || myReaderImpl->pos->NoChildren())
    {
        throw YCException(2002, "YCXmlReader::foreach pos is null!");
    }

	tinyxml2::XMLNode* cur = myReaderImpl->pos;
	try
	{
		for (tinyxml2::XMLElement *e = cur->FirstChildElement(node);
			 NULL != e;
			 e = e->NextSiblingElement(node))
		{
			myReaderImpl->pos = e;
			if (!wrapper.load(*this))
			{
				SMART_ASSERT(false);
			}
		}
	}
	catch (YCException& e)
	{
		UNREFERENCED_PARAMETER(e);
		myReaderImpl->pos = cur;
		// throw(e);
	}
	
	myReaderImpl->pos = cur;

    return *this;
}

//
// 函数：leave()
//
// 目的：回到父节点
//
YCXmlReader& YCXmlReader::leave()
{
	if (NULL == myReaderImpl->pos)
    {
        throw YCException(2002, "YCXmlReader::leave pos is null!");
    }

    tinyxml2::XMLNode *parent = myReaderImpl->pos->Parent();
    if (NULL == parent)
    {
        throw YCException(2002, "YCXmlReader::leave Parent is null!");
    }
    myReaderImpl->pos = parent;
	return *this;
}

const char *YCXmlReader::get_value()
{
	if (NULL == myReaderImpl->pos)
    {
        throw YCException(2002, "YCXmlReader::get_value pos is null!");
    }

    tinyxml2::XMLElement *e = myReaderImpl->pos->ToElement();
    if (NULL == e)
    {
        throw YCException(2002, "YCXmlReader::get_value invalid element!");
    }
    const char *val = e->GetText();
    if (NULL == val)
    {
        throw YCException(2002, "YCXmlReader::get_value has no value!");
    }
	return val;
}
