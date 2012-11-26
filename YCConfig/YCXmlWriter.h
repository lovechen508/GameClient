#ifndef _INCLUDE_YCXMLWRITER_H_
#define _INCLUDE_YCXMLWRITER_H_

class YCXmlWriterImpl;

#include "YCBasic\YCAssert.h"

/*
 * YCXmlWriter : Xml配置文件写入类
 *
 * 说明：
 *      
 *		YCXmlWriter writer("c:\\liao.xml");
 *		
 *		writer.enter("student", &YCXmlWriter::YCValueFilter("廖赞旭", NULL))
 *			    .attr("sex", "male")
 *		        .attr("age", 45)
 *			  .leave()
 *		      .enter("blacklist", NULL)
 *			    .enter("role", &YCXmlWriter::YCValueFilter("knuth", NULL))
 *				  .attr("level", 20).attr("country", "中国")
 *				.leave()
 *				.enter("role", &YCXmlWriter::YCValueFilter("Andy", NULL))
 *				  .attr("level", 30).attr("country", "澳大利亚")
 *				.leave()
 *				.remove(&YCXmlWriter::YCNameFilter("role", &YCXmlWriter::YCValueFilter("亚历山大", NULL)))
 *				.append("role", "亚历山大")
 *		      .leave()
 *			  .enter("blacklist", NULL)
 *			    .remove(&YCXmlWriter::YCNameFilter("role", &YCXmlWriter::YCAttrFilter("country", "澳大利亚", NULL)))
 *			  .leave()
 *		      .flush();
 *
 */
class YCAPI YCXmlWriter
{
private:

	//
	// YCForEachWrapper foreach functor 包装器接口
	//
	class YCIForEachWrapper
	{
	public:
		
		virtual bool load(YCXmlWriter& writer) = 0;

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

		YCForEachWrapper(T* t, bool (T::*load)(YCXmlWriter&))
			:handler(t), execute(load)
		{
			SMART_ASSERT(t != NULL && load != NULL);
		}

		virtual ~YCForEachWrapper() {}

		virtual bool load(YCXmlWriter& writer)
		{
			return (handler->*execute)(writer);
		}

	private:
		T* handler;
		bool (T::*execute)(YCXmlWriter&);
	};

	//
	// YCForEachWrapper1: foreach functor 包装器模版类实现
	//
	template<typename T, typename P>
	class YCForEachWrapper1 : public YCIForEachWrapper
	{
	public:

		YCForEachWrapper1(T* t, bool (T::*load)(YCXmlWriter&, P*), P* p)
			:handler(t), execute(load), payload(p)
		{
			SMART_ASSERT(t != NULL && load != NULL);
		}

		virtual ~YCForEachWrapper1() {}

		virtual bool load(YCXmlWriter& writer)
		{
			return (handler->*execute)(writer, payload);
		}

	private:
		T* handler;
		P* payload;
		bool (T::*execute)(YCXmlWriter&, P*);
	};

public:

	/*
	 * YCIFilter : 节点查找过滤器
	 *
	 * 说明：
	 *       在Filter中出现的字符串由客户持有生命周期
	 *       并且保证在Filter生命周期内有效
	 *       所有的过滤器都不持有生命周期，建议都在栈上创建
	 *       例如：enter("role", &YCValueFilter("knuth", &YCAttrFilter("sex", "male", NULL)))
	 */
	class YCIXmlFilter
	{
	public:

		YCIXmlFilter(YCIXmlFilter *next)
			: myNext(next)
		{
		}

		virtual ~YCIXmlFilter() {}

		//
		// 函数：apply(void *doc, void * newElement)
		//
		// 目的：给新节点补齐过滤器条件
		//
		virtual void apply(void *doc, void * newElement)
		{
			selfApply(doc, newElement);
			if (NULL != myNext)
			{
				myNext->apply(doc, newElement);
			}
		}

		//
		// 函数：match(void *node)
		//
		// 目的：节点是否匹配条件
		//
		bool match(void *node)
		{
			if (selfMatch(node))
			{
				return (myNext == NULL) ? true : myNext->match(node);
			}
			return false;
		}

	private:

		//
		// 函数：selfApply(void *doc, void * newElement)
		//
		// 目的：给新节点补齐过滤器条件
		//
		virtual void selfApply(void *doc, void * newElement) = 0;

		//
		// 函数：selfMatch(void *node)
		//
		// 目的：自身匹配
		//
		virtual bool selfMatch(void *node) = 0;

		//
		// 属性：后续过滤器
		//
		YCIXmlFilter *myNext;
	};

	/*
	 * YCNameFilter : 名称过滤器
	 */
	class YCNameFilter : public YCIXmlFilter
	{
	public:

		YCNameFilter(const char* name, YCIXmlFilter* next)
			: YCIXmlFilter(next), myName(name)
		{
			SMART_ASSERT(myName != NULL);
		}

		virtual ~YCNameFilter() {}

	private:
		
		//
		// 函数：selfApply(void *doc, void * newElement)
		//
		// 目的：给新节点补齐过滤器条件
		//
		virtual void selfApply(void *doc, void * newElement);

		//
		// 函数：selfMatch(void *node)
		//
		// 目的：自身匹配
		//
		virtual bool selfMatch(void *node);

	private:

		const char* myName;
	};

	/*
	 * YCAttrFilter : 属性过滤器
	 */
	class YCAttrFilter : public YCIXmlFilter
	{
	public:

		YCAttrFilter(const char* name, const char* value, YCIXmlFilter* next)
			: YCIXmlFilter(next), myName(name), myValue(value)
		{
			SMART_ASSERT(myName != NULL);
		}

		virtual ~YCAttrFilter() {}

	private:
		
		//
		// 函数：selfApply(void *doc, void * newElement)
		//
		// 目的：给新节点补齐过滤器条件
		//
		virtual void selfApply(void *doc, void * newElement);

		//
		// 函数：selfMatch(void *node)
		//
		// 目的：自身匹配
		//
		virtual bool selfMatch(void *node);

	private:

		const char* myName;
		const char* myValue;
	};

	/*
	 * YCValueFilter : 值过滤器
	 */
	class YCValueFilter : public YCIXmlFilter
	{
	public:

		YCValueFilter(const char* value, YCIXmlFilter* next)
			: YCIXmlFilter(next), myValue(value)
		{
			SMART_ASSERT(myValue != NULL);
		}
		
		virtual ~YCValueFilter() {}

	private:

		//
		// 函数：selfApply(void *doc, void * newElement)
		//
		// 目的：给新节点补齐过滤器条件
		//
		virtual void selfApply(void *doc, void * newElement);

		//
		// 函数：selfMatch(void *node)
		//
		// 目的：自身匹配
		//
		virtual bool selfMatch(void *node);

	private:

		const char* myValue;
	};

public:
	
	YCXmlWriter(const char* filename);
	
	~YCXmlWriter(void);

	//
	// 函数：validate()
	//
	// 目的：校验XML文档的完整性
	//
	bool validate();

	// 
	// 函数：enter(const char* name, YCIXmlFilter* filter)
	//
	// 目的：进入子节点
	//
	YCXmlWriter& enter(const char* name, YCIXmlFilter* filter);

	//
	// 函数：foreach(const char* name, YCIXmlFilter* filter, YCIForEachWrapper& wrapper)
	//
	// 目的：遍历名为node的子节点
	//
	YCXmlWriter& foreach(const char* name, YCIXmlFilter* filter, YCIForEachWrapper& wrapper);

	//
	// 函数：value(const char* val)
	//
	// 目的：写字符串值
	//
	YCXmlWriter& value(const char* val);

	//
	// 函数：value(int& val)
	//
	// 目的：写int型值
	//
	YCXmlWriter& value(int val);
	
	//
	// 函数：attr(const char *name, const char* str)
	//
	// 目的：写字符串属性值
	//
	YCXmlWriter& attr(const char *name, const char* val);

	//
	// 函数：attr(const char *name, int str)
	//
	// 目的：写int型属性值
	//
	YCXmlWriter& attr(const char *name, int val);

	//
	// 函数：append(const char *name, const char* val)
	//
	// 目的：追加节点
	//
	YCXmlWriter& append(const char *name, const char* val);

	//
	// 函数：append(const char *name, int val)
	//
	// 目的：追加节点
	//
	YCXmlWriter& append(const char *name, int val);

	//
	// 函数：removeAttr(const char* name)
	//
	// 目的：移除名为name的属性
	//
	YCXmlWriter& removeAttr(const char* name);

	//
	// 函数：remove(YCIXmlFilter* filter)
	//
	// 目的：移除filter为true的子节点
	//       filter为NULL，移除所有子节点
	//
	YCXmlWriter& remove(YCIXmlFilter* filter);
	
	//
	// 函数：leave()
	//
	// 目的：回到父节点
	//
	YCXmlWriter& leave();

	//
	// 函数：flush()
	//
	// 目的：持久化文件
	//
	void flush();

private:

	//
	// 属性：文件名
	//
	char * myFilename;

	//
	// 属性：写入器内部实现类
	//
	YCXmlWriterImpl *myWriterImpl;
};

#endif