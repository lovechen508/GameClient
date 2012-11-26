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
// CSS文件每一行的类型
//
enum E_CSS_LINE_TYPE 
{
	LINE_INVALID		= -1,
	LINE_COMMENTS		= 0,	// #打头
	LINE_SELECTOR       = 1,    // 选择符
	LINE_BRACKET_START  = 2,	// {
	LINE_PROPERTY       = 3,	//	  width : 100	
	LINE_BRACKET_END    = 4,	// }
	LINE_EMPTY          = 5,    // 空行
};

//
// 帮助函数
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
// 函数：parse(YCCSSContext* context, const char * filename)
//
// 目的：解析CSS文件，构建YCCSSContext
//
bool YCCSSParser::parse(YCCSSContext* context, const char * filename)
{
	SMART_ASSERT(filename != NULL);
	if (filename == NULL)
	{
		throw YCException(2002, "YCCSSParser::parse文件名为NULL");
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
							throw YCException(2002, "YCCSSParser::parse解析文件失败，LINE_SELECTOR CSS节点未正确结束");
						}
						node = new YCCSSNode(trimmed);
					}
					break;
				case LINE_BRACKET_START:
					{
						if (node == NULL)
						{
							throw YCException(2002, "YCCSSParser::parse解析文件失败，LINE_BRACKET_START CSS节点未正确开始");
						}
					}
					break;
				case LINE_PROPERTY:
					{
						if (node == NULL)
						{
							throw YCException(2002, "YCCSSParser::parse解析文件失败，LINE_PROPERTY CSS节点未正确开始");
						}

						std::vector<std::string> strVector;
						YCStringHelper::split(trimmed, ":", &strVector);
						if (strVector.size() != 2)
						{
							LOG_WARNING("YCCSSParser::parse解析文件" << filename << "属性错误：" << trimmed);
							throw YCException(2002, "YCCSSParser::parse解析文件失败，LINE_PROPERTY CSS节点属性错误");
						}

						E_UITAG_CSS property = YCUITagHelper::Instance().getPropertyEnum(strVector[0].c_str());
						if (property != UITAG_INVALID)
						{
							YCCSSItem *cssItem = new YCCSSItem(property);
							const std::string& value = strVector[1].c_str();
							if (value[value.length()-1] != ';')
							{
								LOG_WARNING("YCCSSParser::parse解析文件" << filename << "属性值错误：" << value);
								throw YCException(2002, "YCCSSParser::parse解析文件失败，LINE_PROPERTY CSS节点属性未正确结束");
							}

							cssItem->setValue(value.substr(0, value.length()-1).c_str());
							node->setProperty(property, cssItem);
						}
						else
						{
							LOG_WARNING("YCCSSParser::parse解析文件" << filename << "属性" << strVector[0] << "值错误：" << strVector[1]);
						}
					}
					break;
				case LINE_BRACKET_END:
					{
						if (node == NULL)
						{
							throw YCException(2002, "YCCSSParser::parse解析文件失败，LINE_BRACKET_END CSS节点未正确结束");
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
			LOG_FATAL("YCCSSParser::parse解析文件" << filename << "失败：" << e.what());
			return false;
		}
		catch ( ... )
		{
			LOG_FATAL("YCCSSParser::parse解析文件" << filename << "失败：未知异常");
			return false;
		}
	}
	return true;
}
