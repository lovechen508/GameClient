#include "YCCSSParser.h"

#include <fstream>
#include <stdlib.h>

#include "YCCSSItem.h"
#include "YCCSSNode.h"
#include "YCCSSContext.h"
#include "YCUITagHelper.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCStringHelper.h"

//
// CSS�ļ�ÿһ�е�����
//
enum E_CSS_LINE_TYPE 
{
	LINE_INVALID		= -1,
	LINE_COMMENTS		= 0,	// #��ͷ
	LINE_SELECTOR       = 1,    // ѡ���
	LINE_BRACKET_START  = 2,	// {
	LINE_PROPERTY       = 3,	//	  width : 100	
	LINE_BRACKET_END    = 4,	// }
	LINE_EMPTY          = 5,    // ����
};

//
// ��������
//
E_CSS_LINE_TYPE checkLine(const std::string& trimmed)
{
	if      (trimmed.size() == 0) 
	{
		return LINE_EMPTY;
	}
	else if (trimmed.find(":hover") != std::string::npos)
	{
		return LINE_SELECTOR;
	}
	else if (trimmed[0] == '-' && trimmed.size() >= 2 && trimmed[1] == '-')
	{
		return LINE_COMMENTS;
	}
	else if (trimmed[0] == '{')
	{
		return LINE_BRACKET_START;
	}
	else if (trimmed[0] == '}') 
	{
		return LINE_BRACKET_END;
	}
	else if (trimmed.find_first_of(':') != std::string::npos)
	{
		return LINE_PROPERTY;
	}
	
	return LINE_SELECTOR;
}

//
// ������parse(YCCSSContext* context, const char * filename)
//
// Ŀ�ģ�����CSS�ļ�������YCCSSContext
//
bool YCCSSParser::parse(YCCSSContext* context, const char * filename)
{
	SMART_ASSERT(filename != NULL);
	if (filename == NULL)
	{
		throw YCException(2002, "YCCSSParser::parse�ļ���ΪNULL");
	}

	std::fstream fs(filename, std::ios::in); //readonly
	if (fs.good())
	{
		try 
		{
			const int LINE_LENGTH = 1024; 
			char buf[LINE_LENGTH] = {0};

			YCCSSNode *node = NULL;
			while (fs.getline(buf, LINE_LENGTH))
			{
				const char* trimmed = YCStringHelper::trim(buf);				
				E_CSS_LINE_TYPE type = checkLine(trimmed);
				LOG_TRACE(filename << ": " << trimmed << "type: " << type);
				switch (type)
				{
				case LINE_SELECTOR:
					{
						if (node != NULL)
						{
							throw YCException(2002, "YCCSSParser::parse�����ļ�ʧ�ܣ�LINE_SELECTOR CSS�ڵ�δ��ȷ����");
						}
						node = new YCCSSNode(trimmed);
					}
					break;
				case LINE_BRACKET_START:
					{
						if (node == NULL)
						{
							throw YCException(2002, "YCCSSParser::parse�����ļ�ʧ�ܣ�LINE_BRACKET_START CSS�ڵ�δ��ȷ��ʼ");
						}
					}
					break;
				case LINE_PROPERTY:
					{
						if (node == NULL)
						{
							throw YCException(2002, "YCCSSParser::parse�����ļ�ʧ�ܣ�LINE_PROPERTY CSS�ڵ�δ��ȷ��ʼ");
						}

						std::vector<std::string> strVector;
						YCStringHelper::split(trimmed, ":", &strVector);
						if (strVector.size() != 2)
						{
							LOG_WARNING("YCCSSParser::parse�����ļ�" << filename << "���Դ���" << trimmed);
							throw YCException(2002, "YCCSSParser::parse�����ļ�ʧ�ܣ�LINE_PROPERTY CSS�ڵ����Դ���");
						}

						E_UITAG_CSS property = YCUITagHelper::Instance().getPropertyEnum(strVector[0].c_str());
						if (property != UITAG_INVALID)
						{
							YCCSSItem *cssItem = new YCCSSItem(property);
							const std::string& value = strVector[1].c_str();
							if (value[value.length()-1] != ';')
							{
								LOG_WARNING("YCCSSParser::parse�����ļ�" << filename << "����ֵ����" << value);
								throw YCException(2002, "YCCSSParser::parse�����ļ�ʧ�ܣ�LINE_PROPERTY CSS�ڵ�����δ��ȷ����");
							}

							cssItem->setValue(value.substr(0, value.length()-1).c_str());
							node->setProperty(property, cssItem);
						}
						else
						{
							LOG_WARNING("YCCSSParser::parse�����ļ�" << filename << "����" << strVector[0] << "ֵ����" << strVector[1]);
						}
					}
					break;
				case LINE_BRACKET_END:
					{
						if (node == NULL)
						{
							throw YCException(2002, "YCCSSParser::parse�����ļ�ʧ�ܣ�LINE_BRACKET_END CSS�ڵ�δ��ȷ����");
						}
						node->calculatePriority();
						context->addNode(node);
						node = NULL;
					}
					break;

				case LINE_COMMENTS:
				case LINE_EMPTY:
				default:
					break;
				};
			};
		}
		catch (YCException& e)
		{
			LOG_FATAL("YCCSSParser::parse�����ļ�" << filename << "ʧ�ܣ�" << e.what());
			return false;
		}
		catch ( ... )
		{
			LOG_FATAL("YCCSSParser::parse�����ļ�" << filename << "ʧ�ܣ�δ֪�쳣");
			return false;
		}
	}
	return true;
}
