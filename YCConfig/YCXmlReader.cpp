#include "YCXmlReader.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCException.h"

#include "tinyxml2.h"

#include <ctype.h>

//
// ������isInt(const char *tocheck)
//
// Ŀ�ģ�ֵ�Ƿ�Ϊinteger
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
 * YCXmlReaderImpl : �ڲ�������
 *  
 * Ŀ��: �Ե������ļ򵥰�װ
 *       �Կͻ���ȫ���ε�����xml parser
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
// ������YCXmlReader(const char *file)
//
// Ŀ�ģ����캯�������ļ�����
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
// ������YCXmlReader(const char* buffer, unsigned int length)
//
// Ŀ�ģ����캯�������ڴ�δ���
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
// ������validate()
//
// Ŀ�ģ�У��XML�ĵ���������
//
bool YCXmlReader::validate()
{
   // throw YCException(2002, "Not Implement yet!");
    return true;
}

// 
// ������enter(const char* node)
//
// Ŀ�ģ������ӽڵ�
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
// ������value(char** val)
//
// Ŀ�ģ�ȡ�ַ���ֵ��val���ڴ����������
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
// ������value(char** val, const char* defaultValue)
//
// Ŀ�ģ�ȡ�ַ���ֵ��val���ڴ����������
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
// ������value(int& val)
//
// Ŀ�ģ�ȡint��ֵ
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
// ������value(int& val, int defaultValue)
//
// Ŀ�ģ�ȡint��ֵ
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
// ������value(unsigned int& val)
//
// Ŀ�ģ�ȡunsigned int��ֵ
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
// ������value(unsigned int& val, unsigned int defaultValue)
//
// Ŀ�ģ�ȡint��ֵ
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
// ������attr(const char *name, char** str)
//
// Ŀ�ģ�ȡ�ַ�������ֵ��val���ڴ����������
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
// ������attr(const char *name, char** str, const char* defaultValue)
//
// Ŀ�ģ�ȡ�ַ�������ֵ��val���ڴ����������
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
// ������attr(const char *name, int& str)
//
// Ŀ�ģ�ȡint������ֵ
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
// ������attr(const char *name, int& str, int defaultValue)
//
// Ŀ�ģ�ȡint������ֵ
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
// ������attr(const char *name, unsigned int& str)
//
// Ŀ�ģ�ȡint������ֵ
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
// ������attr(const char *name, int& str, unsigned int defaultValue)
//
// Ŀ�ģ�ȡint������ֵ
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
// ������foreach(const char *node, YCIForEachWrapper& wrapper)
//
// Ŀ�ģ�������Ϊnode���ӽڵ�
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
// ������leave()
//
// Ŀ�ģ��ص����ڵ�
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
