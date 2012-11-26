#ifndef _INCLUDE_YCXMLREADER_H_
#define _INCLUDE_YCXMLREADER_H_

#include <string>
#include "YCBasic\YCAssert.h"

class YCXmlReaderImpl;

/*
 * YCXmlReader : XML 配置文件阅读类
 *
 * 注释：因为涉及到中文
 *       XML文件一定要保存为UTF-8格式
 */

class YCAPI YCXmlReader 
{
private:

	//
	// YCForEachWrapper foreach functor 包装器接口
	//
	class YCIForEachWrapper
	{
	public:
		
		virtual bool load(YCXmlReader& reader) = 0;

	protected:
		YCIForEachWrapper() {}
		virtual ~YCIForEachWrapper() {}
	};

public:

	//
	// YCForEachWrapper: foreach functor 包装器模版类实现
	//
	template<typename T>
	class YCForEachWrapper : public YCIForEachWrapper
	{
	public:

		YCForEachWrapper(T* t, bool (T::*load)(YCXmlReader&))
			:handler(t), execute(load)
		{
			SMART_ASSERT(t != NULL && load != NULL);
		}

		virtual ~YCForEachWrapper() {}

		virtual bool load(YCXmlReader& reader)
		{
			return (handler->*execute)(reader);
		}

	private:
		T* handler;
		bool (T::*execute)(YCXmlReader&);
	};

	//
	// YCForEachWrapper1: foreach functor 包装器模版类实现
	//
	template<typename T, typename P>
	class YCForEachWrapper1 : public YCIForEachWrapper
	{
	public:

		YCForEachWrapper1(T* t, bool (T::*load)(YCXmlReader&, P*), P* p)
			:handler(t), execute(load), payload(p)
		{
			SMART_ASSERT(t != NULL && load != NULL);
		}

		virtual ~YCForEachWrapper1() {}

		virtual bool load(YCXmlReader& reader)
		{
			return (handler->*execute)(reader, payload);
		}

	private:
		T* handler;
		P* payload;
		bool (T::*execute)(YCXmlReader&, P*);
	};

public:

	//
	// 函数：YCXmlReader(const char *file)
	//
	// 目的：构造函数，从文件创建
	//
	YCXmlReader(const char *file);

	//
	// 函数：YCXmlReader(const char* buffer, unsigned int length)
	//
	// 目的：构造函数，从内存段创建
	//
	YCXmlReader(const char* buffer, unsigned int length);

	//
	// 析构：释放内部结构
	//
	~YCXmlReader(void);

	//
	// 函数：validate()
	//
	// 目的：校验XML文档的完整性
	//
    bool validate();

    // 
	// 函数：enter(const char* node)
	//
	// 目的：进入子节点
	//
    YCXmlReader& enter(const char* node);
    
	//
	// 函数：value(char** val)
	//
	// 目的：取字符串值，val的内存在里面分配
	//
	YCXmlReader& value(char** val);

	//
	// 函数：value(char** val, const char* defaultValue)
	//
	// 目的：取字符串值，val的内存在里面分配
	//
	YCXmlReader& value(char** val, const char* defaultValue);

	//
	// 函数：value(int& val)
	//
	// 目的：取int型值
	//
    YCXmlReader& value(int& val);

	//
	// 函数：value(int& val, int defaultValue)
	//
	// 目的：取int型值
	//
    YCXmlReader& value(int& val, int defaultValue);

	//
	// 函数：value(unsigned int& val)
	//
	// 目的：取unsigned int型值
	//
	YCXmlReader& value(unsigned int& val);

	//
	// 函数：value(unsigned int& val, unsigned int defaultValue)
	//
	// 目的：取int型值
	//
	YCXmlReader& value(unsigned int& val, unsigned int defaultValue);

	//
	// 函数：attr(const char *name, char** str)
	//
	// 目的：取字符串属性值，val的内存在里面分配
	//
	YCXmlReader& attr(const char *name, char** val);

	//
	// 函数：attr(const char *name, char** str, const char* defaultValue)
	//
	// 目的：取字符串属性值，val的内存在里面分配
	//
	YCXmlReader& attr(const char *name, char** val, const char* defaultValue);

	//
	// 函数：attr(const char *name, int& str)
	//
	// 目的：取int型属性值
	//
    YCXmlReader& attr(const char *name, int& val);

	//
	// 函数：attr(const char *name, int& str, int defaultValue)
	//
	// 目的：取int型属性值
	//
    YCXmlReader& attr(const char *name, int& val, int defaultValue);

	//
	// 函数：attr(const char *name, unsigned int& str)
	//
	// 目的：取int型属性值
	//
	YCXmlReader& attr(const char *name, unsigned int& val);

	//
	// 函数：attr(const char *name, unsigned int& str, unsigned int defaultValue)
	//
	// 目的：取int型属性值
	//
	YCXmlReader& attr(const char *name, unsigned int& val, unsigned int defaultValue);

    //
	// 函数：foreach(const char *node, YCIForEachWrapper& wrapper)
	//
	// 目的：遍历名为node的子节点
	//
    YCXmlReader& foreach(const char *node, YCIForEachWrapper& wrapper);
 
	//
	// 函数：leave()
	//
	// 目的：回到父节点
	//
    YCXmlReader& leave();

private:

	const char *get_value();

private:

    char * filename;
	YCXmlReaderImpl *myReaderImpl;
};


#endif
