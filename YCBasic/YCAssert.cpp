#include "YCAssert.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "YCLogger.h"
#include "YCException.h"

#pragma region YCAssertContext

class YCAssertContext
{
	typedef std::pair<std::string, std::string> ValueAndString_t;
	typedef std::vector<ValueAndString_t>		ValsArray_t;

public:
	YCAssertContext()
		: m_level(LVL_ERROR)
		, m_line(0)
	{
	}

	void setLevel(const E_ASSERTLEVEL lvl)
	{
		m_level = lvl;
	}

	E_ASSERTLEVEL getLevel() const
	{
		return m_level;
	}

	void setFile(const char* file)
	{
		m_file = file;
	}

	void setFile(const std::string& file)
	{
		m_file = file;
	}

	const std::string& getFile() const
	{
		return m_file;
	}

	void setLine(const unsigned int line)
	{
		m_line = line;
	}

	unsigned int getLine() const
	{
		return m_line;
	}

	void setDesc(const char* desc)
	{
		m_desc = desc;
	}

	void setDesc(const std::string& desc)
	{
		m_desc = desc;
	}

	const std::string& getDesc() const
	{
		return m_desc;
	}

	void setExpr(const char* expr)
	{
		m_expr = expr;
	}

	void setExpr(const std::string& expr)
	{
		m_expr = expr;
	}

	const std::string& getExpr() const
	{
		return m_expr;
	}

	void addValue(const char* name, const std::string& value)
	{
		m_values.push_back(std::make_pair(name, value));
	}

	void addValue(const std::string& name, const std::string& value)
	{
		m_values.push_back(std::make_pair(name, value));
	}
        
	const ValsArray_t& getValues() const
	{
		return m_values;
	}

	const std::string getMsg() const
	{
		std::ostringstream out;
		try{
			out << "[" << m_file << ":" << m_line << "] ";
            
			/*
			switch (m_level){
				case LVL_WARN:	out << "Warn: ";	break;
				case LVL_ERROR:	out << "Error: ";	break;
				case LVL_FATAL:	out << "Fatal: ";	break;
				default:		out << "Unknown: ";	break;
			}
			*/
			
			out << m_desc << std::endl;
			out << m_expr << " failed" << std::endl;
			for(ValsArray_t::const_iterator it = m_values.begin(); it != m_values.end(); ++it)
					out << (*it).first << " = " << (*it).second << std::endl;
			out << std::flush;
		}
		catch(...)
		{
			// guarantee stability
		}
		return out.str();
	}

protected:

	E_ASSERTLEVEL	m_level;
	std::string		m_file;
	unsigned int	m_line;
	std::string		m_desc;
	std::string		m_expr;
	ValsArray_t		m_values;       
};

#pragma endregion YCAssertContext

#pragma region YCAssertException

/////////////////////////////////////////////////////
// as usual exception, but keep context of assertion 
struct YCAssertException
	: public YCException 
{                                                
	YCAssertException(int err, const char *desc)
		: YCException(err, desc)
	{
	}
};

#pragma endregion YCAssertException

YCAssert::HandlerCallback_t YCAssert::m_callbacks[LVL_TOTAL] =
        {defaultWarnHandler, defaultErrorHandler, defaultFatalHandler};

#undef SMART_ASSERT_A
#undef SMART_ASSERT_B

#pragma warning(push)
#pragma warning(disable : 4355)  // warning C4355: 'this' : used in base member initializer list

YCAssert::YCAssert(const char* expr)
	: m_context(new YCAssertContext())
	, SMART_ASSERT_A(*this)
	, SMART_ASSERT_B(*this)
{
	m_context->setExpr(expr);
}

#pragma warning(pop)    // C4355

YCAssert::~YCAssert()
{
	const E_ASSERTLEVEL level = m_context->getLevel();
	if ((level >= LVL_WARN) && (level < LVL_TOTAL))
	{
		if (m_callbacks[level]) 
		{
			m_callbacks[level](*m_context);
		}
		delete m_context;
	}
	else
	{
		abort();        // something gonna very bad
	}
}

YCAssert& YCAssert::addValue(const char* name, int value)
{
	char buf[24] = {0};
	if (_itoa_s(value, buf, 10) != 0)
	{
		LOG_ERROR("YCAssert::addValueÉèÖÃInt´íÎóÊ§°Ü£º" << value);
	}
	else 
	{
		m_context->addValue(name, buf);
	}
	return (*this);
}

YCAssert& YCAssert::addValue(const char* name, float value)
{
	char buf[24] = {0};
	sprintf_s(buf,"%f",value);  // float to char
	m_context->addValue(name, buf);
	return (*this);
}

YCAssert& YCAssert::addValue(const char* name, double value)
{
	char buf[24] = {0};
	sprintf_s(buf,"%lf",value);  // float to char
	m_context->addValue(name, buf);
	return (*this);
}

YCAssert& YCAssert::addValue(const char* name, const char* value)
{
	m_context->addValue(name, value);
	return (*this);
}

YCAssert& YCAssert::setContext(const char* file, const unsigned int line)
{
#if defined(WIN32)
	static const char* dir_separator = "\\";
#else
	static const char* dir_separator = "/";
#endif // defined(WIN32)

	std::string sfile(file);
	const std::string::size_type nPos = sfile.find_last_of(dir_separator);
	m_context->setFile((nPos == std::string::npos) ? file : sfile.substr(nPos + 1));
	m_context->setLine(line);
	return (*this);
}

YCAssert& YCAssert::setExpr(const char* expr)
{
	m_context->setExpr(expr);      
	return (*this);
}

YCAssert& YCAssert::warn(const char* desc)
{
	m_context->setLevel(LVL_WARN);
	m_context->setDesc(desc);
	return (*this);
}

YCAssert& YCAssert::error(const char* desc)
{
	m_context->setLevel(LVL_ERROR);
	m_context->setDesc(desc);
	return (*this);
}

YCAssert& YCAssert::fatal(const char* desc)
{
	m_context->setLevel(LVL_FATAL);
	m_context->setDesc(desc);
	return (*this);
}

void YCAssert::defaultWarnHandler(const YCAssertContext& context)
{
	LOG_WARNING("YCAssert : " << context.getMsg());
}

void YCAssert::defaultErrorHandler(const YCAssertContext& context)
{
	const std::string& msg = context.getMsg();
	LOG_ERROR("YCAssert : " << msg.c_str());
	throw YCAssertException(1001, msg.c_str());
}

void YCAssert::defaultFatalHandler(const YCAssertContext& context)
{
	LOG_FATAL("YCAssert : " << context.getMsg());
	abort();
}

void YCAssert::setHandler(E_ASSERTLEVEL level, HandlerCallback_t callback)
{
	SMART_ASSERT((level >= LVL_WARN) && (level < LVL_TOTAL))
		.error("Bad assertion level.").addValue("level", level);

	SMART_ASSERT(callback != NULL)
		.warn("Assertion handling for specified level will be disabled")
		.addValue("level", level);

	m_callbacks[level] = callback;
}
