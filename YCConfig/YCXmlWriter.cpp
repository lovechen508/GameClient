#include "YCXmlWriter.h"

#include <string>

#include "tinyxml2.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"

#define VALIDATE_POS if (NULL == myWriterImpl->pos) \
					 {\
						throw YCException(2002, "节点异常", __FUNCTION__);\
					 }

using namespace tinyxml2;

///////////////////////////////////////////////////////
// YCXmlWriterImpl 内部实现类
///////////////////////////////////////////////////////

class YCXmlWriterImpl
{
public:

	YCXmlWriterImpl(const char* file)
		: filename(file)
		, doc(NULL)
		, pos(NULL)
	{
		doc = new tinyxml2::XMLDocument();
		if (YCFileUtil::IsFileExist(filename.c_str()))
		{
			if (tinyxml2::XML_NO_ERROR != doc->LoadFile(filename.c_str()))
			{
				throw YCException(2002, doc->GetErrorStr1());
			}
		}
		else
		{
			XMLDeclaration *decl = doc->NewDeclaration();
			XMLElement *element = doc->NewElement("storage");
			doc->LinkEndChild(decl);
			doc->LinkEndChild(element);
		}
		pos = doc->RootElement();
	}

	~YCXmlWriterImpl() 
	{
		SAFE_DELETE(doc);
	}
	
	//
	// 函数：flush()
	//
	// 目的： 写入到文件
	//
	void flush()
	{
		doc->SaveFile(filename.c_str());
	}

	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLNode *pos;

	std::string filename;
};

//
// 函数：selfApply(void *doc, void * newElement)
//
// 目的：给新节点补齐过滤器条件
//
void YCXmlWriter::YCNameFilter::selfApply(void *doc, void * newElement)
{
	XMLElement *element = (XMLElement*)newElement;
	element->SetName(myName);
}

bool YCXmlWriter::YCNameFilter::selfMatch(void *node)
{
	if (NULL == node)
	{
		throw YCException(2002, "YCXmlWriter::YCAttrFilter::selfMatch invalid node!");
	}

	XMLElement *e = ((XMLNode*)node)->ToElement();
	if (NULL == e)
	{
		throw YCException(2002, "YCXmlWriter::YCAttrFilter::selfMatch invalid element!");
	}

	return (stricmp(myName, e->Name()) == 0);
}

//
// 函数：selfApply(void *doc, void * newElement)
//
// 目的：给新节点补齐过滤器条件
//
void YCXmlWriter::YCAttrFilter::selfApply(void *doc, void * newElement)
{
	XMLElement *element = (XMLElement*)newElement;
	element->SetAttribute(myName, myValue);
}

bool YCXmlWriter::YCAttrFilter::selfMatch(void *node)
{
	if (NULL == node)
	{
		throw YCException(2002, "YCXmlWriter::YCAttrFilter::selfMatch invalid node!");
	}

	XMLElement *e = ((XMLNode*)node)->ToElement();
	if (NULL == e)
	{
		throw YCException(2002, "YCXmlWriter::YCAttrFilter::selfMatch invalid element!");
	}

	const char* attr = e->Attribute(myName);
	return ((attr != NULL) && (stricmp(myValue, attr) == 0));
}

//
// 函数：selfApply(void *doc, void * newElement)
//
// 目的：给新节点补齐过滤器条件
//
void YCXmlWriter::YCValueFilter::selfApply(void *doc, void *newElement)
{
	XMLElement *element = (XMLElement*)newElement;
	tinyxml2::XMLDocument *document = (tinyxml2::XMLDocument*)doc;
	XMLText *text = document->NewText(myValue);
	element->LinkEndChild(text);
}

bool YCXmlWriter::YCValueFilter::selfMatch(void *node)
{
	if (NULL == node)
	{
		throw YCException(2002, "YCXmlWriter::YCValueFilter::selfMatch invalid node!");
	}
	XMLElement *e = ((XMLNode*)node)->ToElement();
	if (NULL == e)
	{
		throw YCException(2002, "YCXmlWriter::YCValueFilter::selfMatch invalid element!");
	}
	const char *val = e->GetText();

	return (myValue == NULL) ? (val == NULL) : ((val != NULL) && (stricmp(myValue, val) == 0));
}

////////////////////////////////////////////////////////
// YCXmlWriter 实现
////////////////////////////////////////////////////////

YCXmlWriter::YCXmlWriter(const char* filename)
{
	SMART_ASSERT(filename != NULL);
	myWriterImpl = new YCXmlWriterImpl(filename);
}


YCXmlWriter::~YCXmlWriter(void)
{
	SAFE_DELETE(myWriterImpl);
}

//
// 函数：validate()
//
// 目的：校验XML文档的完整性
//
bool YCXmlWriter::validate()
{
	// TODO ... 
	return true;
}

// 
// 函数：enter(const char* name, YCIXmlFilter* filter)
//
// 目的：进入子节点
//
YCXmlWriter& YCXmlWriter::enter(const char* name, YCIXmlFilter* filter)
{
	VALIDATE_POS;

	bool found = false;
	for (XMLNode* node = myWriterImpl->pos->FirstChildElement(name);
		 node != NULL && !found;
		 node = node->NextSiblingElement(name))
	{
		if ((filter == NULL) || (filter->match(node)))
		{
			myWriterImpl->pos = node;
			found = true;
		}
	}

	if (!found)
	{
		XMLElement *element = myWriterImpl->doc->NewElement(name);
		if (NULL != filter)
		{
			filter->apply(myWriterImpl->doc, element);
		}
		myWriterImpl->pos->LinkEndChild(element);
		myWriterImpl->pos = element;
	}

	return *this;
}


//
// 函数：foreach(const char* name, YCIXmlFilter* filter, YCIForEachWrapper& wrapper)
//
// 目的：遍历名为node的子节点
//
YCXmlWriter& YCXmlWriter::foreach(const char* name, YCIXmlFilter* filter, YCIForEachWrapper& wrapper)
{
	if (NULL == name)
	{
		throw YCException(2002, "YCXmlWriter::foreach invalid name!");
	}

	if (NULL == myWriterImpl->pos || myWriterImpl->pos->NoChildren())
	{
		throw YCException(2002, "YCXmlWriter::foreach pos is null!");
	}

	tinyxml2::XMLNode* cur = myWriterImpl->pos;
	try
	{
		for (XMLElement *e = cur->FirstChildElement(name);
			NULL != e;
			e = e->NextSiblingElement(name))
		{
			if ((filter == NULL) || filter->match(e))
			{
				myWriterImpl->pos = e;
				if (!wrapper.load(*this))
				{
					SMART_ASSERT(false);
				}
			}
		}
	}
	catch (YCException& e)
	{
		myWriterImpl->pos = cur;
		//throw(e);
	}

	myWriterImpl->pos = cur;

	return *this;
}

//
// 函数：value(const char* val)
//
// 目的：写字符串值
//
YCXmlWriter& YCXmlWriter::value(const char* val)
{
	VALIDATE_POS;

	XMLElement *element = ((XMLNode*)myWriterImpl->pos)->ToElement();
	if (NULL == element)
	{
		throw YCException(2002, "YCXmlWriter::YCValueFilter::selfMatch invalid element!");
	}
	XMLText *text = myWriterImpl->doc->NewText(val);
	element->LinkEndChild(text);

	return *this;
}

//
// 函数：value(int val)
//
// 目的：写int型值
//
YCXmlWriter& YCXmlWriter::value(int val)
{
	VALIDATE_POS;

	XMLElement *element = ((XMLNode*)myWriterImpl->pos)->ToElement();
	if (NULL == element)
	{
		throw YCException(2002, "YCXmlWriter::YCValueFilter::selfMatch invalid element!");
	}
	char buf[10] = {0};
	sprintf(buf, "%d", val);
	XMLText *text = myWriterImpl->doc->NewText(buf);
	element->LinkEndChild(text);

	return *this;
}


//
// 函数：attr(const char *name, const char* str)
//
// 目的：写字符串属性值
//
YCXmlWriter& YCXmlWriter::attr(const char *name, const char* val)
{
	VALIDATE_POS;

	XMLElement *e = ((XMLNode*)myWriterImpl->pos)->ToElement();
	if (NULL == e)
	{
		throw YCException(2002, "YCXmlWriter::YCValueFilter::selfMatch invalid element!");
	}
	e->SetAttribute(name,val);

	return *this;
}

//
// 函数：attr(const char *name, int str)
//
// 目的：写int型属性值
//
YCXmlWriter& YCXmlWriter::attr(const char *name, int val)
{
	VALIDATE_POS;

	XMLElement *e = ((XMLNode*)myWriterImpl->pos)->ToElement();
	if (NULL == e)
	{
		throw YCException(2002, "YCXmlWriter::attr invalid element!");
	}
	e->SetAttribute(name,val);

	return *this;
}

//
// 函数：append(const char *name, const char* val)
//
// 目的：追加节点
//
YCXmlWriter& YCXmlWriter::append(const char *name, const char* val)
{
	VALIDATE_POS;

	XMLElement* element = myWriterImpl->doc->NewElement(name);
	if (NULL == element)
	{
		throw YCException(2002, "YCXmlWriter::append invalid element!");
	}

	XMLText* text = myWriterImpl->doc->NewText(val);
	element->LinkEndChild(text);
	myWriterImpl->pos->LinkEndChild(element);

	return *this;
}

//
// 函数：append(const char *name, int val)
//
// 目的：追加节点
//
YCXmlWriter& YCXmlWriter::append(const char *name, int val)
{
	VALIDATE_POS;

	XMLElement* element = myWriterImpl->doc->NewElement(name);
	if (NULL == element)
	{
		throw YCException(2002, "YCXmlWriter::append invalid element!");
	}
	char buf[10] = {0};
	sprintf(buf, "%d", val);
	XMLText* text = myWriterImpl->doc->NewText(buf);
	element->LinkEndChild(text);
	myWriterImpl->pos->LinkEndChild(element);

	return *this;
}

//
// 函数：removeAttr(const char* name)
//
// 目的：移除名为name的属性
//
YCXmlWriter& YCXmlWriter::removeAttr(const char* name)
{
	VALIDATE_POS;

	XMLElement* element = myWriterImpl->pos->ToElement();
	if (NULL == element)
	{
		throw YCException(2002, "YCXmlWriter::removeAttr invalid element!");
	}
	element->DeleteAttribute(name);

	return *this;
}

//
// 函数：remove(YCIXmlFilter* filter)
//
// 目的：移除filter为true的子节点
//       filter为NULL，移除所有子节点
//
YCXmlWriter& YCXmlWriter::remove(YCIXmlFilter* filter)
{
	VALIDATE_POS;

	if (filter == NULL)
	{
		myWriterImpl->pos->DeleteChildren();
	}
	else
	{
		XMLNode* node = myWriterImpl->pos->FirstChild();
		while (node != NULL)
		{
			XMLNode* next = node->NextSibling();
			if (filter->match(node))
			{
				myWriterImpl->pos->DeleteChild(node);
			}
			node = next;
		}
	}
	
	return *this;
}

//
// 函数：leave()
//
// 目的：回到父节点
//
YCXmlWriter& YCXmlWriter::leave()
{
	VALIDATE_POS;

	myWriterImpl->pos = myWriterImpl->pos->Parent();

	return *this;
}

//
// 函数：flush()
//
// 目的：持久化文件
//
void YCXmlWriter::flush()
{
	myWriterImpl->flush();
}
