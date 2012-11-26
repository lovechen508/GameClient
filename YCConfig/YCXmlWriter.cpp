#include "YCXmlWriter.h"

#include <string>

#include "tinyxml2.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"

#define VALIDATE_POS if (NULL == myWriterImpl->pos) \
					 {\
						throw YCException(2002, "�ڵ��쳣", __FUNCTION__);\
					 }

using namespace tinyxml2;

///////////////////////////////////////////////////////
// YCXmlWriterImpl �ڲ�ʵ����
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
	// ������flush()
	//
	// Ŀ�ģ� д�뵽�ļ�
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
// ������selfApply(void *doc, void * newElement)
//
// Ŀ�ģ����½ڵ㲹�����������
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
// ������selfApply(void *doc, void * newElement)
//
// Ŀ�ģ����½ڵ㲹�����������
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
// ������selfApply(void *doc, void * newElement)
//
// Ŀ�ģ����½ڵ㲹�����������
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
// YCXmlWriter ʵ��
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
// ������validate()
//
// Ŀ�ģ�У��XML�ĵ���������
//
bool YCXmlWriter::validate()
{
	// TODO ... 
	return true;
}

// 
// ������enter(const char* name, YCIXmlFilter* filter)
//
// Ŀ�ģ������ӽڵ�
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
// ������foreach(const char* name, YCIXmlFilter* filter, YCIForEachWrapper& wrapper)
//
// Ŀ�ģ�������Ϊnode���ӽڵ�
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
// ������value(const char* val)
//
// Ŀ�ģ�д�ַ���ֵ
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
// ������value(int val)
//
// Ŀ�ģ�дint��ֵ
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
// ������attr(const char *name, const char* str)
//
// Ŀ�ģ�д�ַ�������ֵ
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
// ������attr(const char *name, int str)
//
// Ŀ�ģ�дint������ֵ
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
// ������append(const char *name, const char* val)
//
// Ŀ�ģ�׷�ӽڵ�
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
// ������append(const char *name, int val)
//
// Ŀ�ģ�׷�ӽڵ�
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
// ������removeAttr(const char* name)
//
// Ŀ�ģ��Ƴ���Ϊname������
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
// ������remove(YCIXmlFilter* filter)
//
// Ŀ�ģ��Ƴ�filterΪtrue���ӽڵ�
//       filterΪNULL���Ƴ������ӽڵ�
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
// ������leave()
//
// Ŀ�ģ��ص����ڵ�
//
YCXmlWriter& YCXmlWriter::leave()
{
	VALIDATE_POS;

	myWriterImpl->pos = myWriterImpl->pos->Parent();

	return *this;
}

//
// ������flush()
//
// Ŀ�ģ��־û��ļ�
//
void YCXmlWriter::flush()
{
	myWriterImpl->flush();
}
