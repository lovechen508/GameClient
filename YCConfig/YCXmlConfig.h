#ifndef _INCLUDE_YCXMLCONFIG_H_
#define _INCLUDE_YCXMLCONFIG_H_

#include <string>

#include "tinyxml2.h"

#include "YCBasic\YCException.h"

class YCAPI YCXmlConfig 
{
public:
	YCXmlConfig(const char *filename);

	~YCXmlConfig(void);

    bool validate();

    // accessers
    YCXmlConfig& enter(const char* node);
    
	YCXmlConfig& value(std::string& val);
    YCXmlConfig& value(int& val);

	YCXmlConfig& attr(const char *name, std::string& str);
    YCXmlConfig& attr(const char *name, int& val);
    
    template<typename T>
    YCXmlConfig& foreach(const char *node,
                 T* t,
                 void (T::*handle)(YCXmlConfig&));
 
    YCXmlConfig& leave();

private:
	const char *get_value();

private:
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLNode *pos;
};

template<typename T>
YCXmlConfig& YCXmlConfig::foreach(const char *node,
        T *t, void (T::*handle)(YCXmlConfig&))
{
	if (NULL == node || NULL == t || NULL == handle)
	{
		throw YCException(2002, "FGPXmlReader::foreach invalid in-params!");
	}

	if (NULL == pos || pos->NoChildren())
    {
        throw YCException(2002, "FGPXmlReader::foreach pos is null!");
    }

	XMLNode* cur = pos;
	try
	{
		for (XMLElement *e = cur->FirstChildElement(node);
			 NULL != e;
			 e = e->NextSiblingElement(node))
		{
			pos = e;
			(t->*handle)(*this);
		}
	}
	catch (YCException& e)
	{
		pos = cur;
		throw;
	}
	
	pos = cur;

    return *this;
}

#endif
