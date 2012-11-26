#ifndef _INCLUDE_YCXMLWRITER_H_
#define _INCLUDE_YCXMLWRITER_H_

class YCXmlWriterImpl;

#include "YCBasic\YCAssert.h"

/*
 * YCXmlWriter : Xml�����ļ�д����
 *
 * ˵����
 *      
 *		YCXmlWriter writer("c:\\liao.xml");
 *		
 *		writer.enter("student", &YCXmlWriter::YCValueFilter("������", NULL))
 *			    .attr("sex", "male")
 *		        .attr("age", 45)
 *			  .leave()
 *		      .enter("blacklist", NULL)
 *			    .enter("role", &YCXmlWriter::YCValueFilter("knuth", NULL))
 *				  .attr("level", 20).attr("country", "�й�")
 *				.leave()
 *				.enter("role", &YCXmlWriter::YCValueFilter("Andy", NULL))
 *				  .attr("level", 30).attr("country", "�Ĵ�����")
 *				.leave()
 *				.remove(&YCXmlWriter::YCNameFilter("role", &YCXmlWriter::YCValueFilter("����ɽ��", NULL)))
 *				.append("role", "����ɽ��")
 *		      .leave()
 *			  .enter("blacklist", NULL)
 *			    .remove(&YCXmlWriter::YCNameFilter("role", &YCXmlWriter::YCAttrFilter("country", "�Ĵ�����", NULL)))
 *			  .leave()
 *		      .flush();
 *
 */
class YCAPI YCXmlWriter
{
private:

	//
	// YCForEachWrapper foreach functor ��װ���ӿ�
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
	// YCForEachWrapper: foreach functor ��װ��ģ����ʵ��
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
	// YCForEachWrapper1: foreach functor ��װ��ģ����ʵ��
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
	 * YCIFilter : �ڵ���ҹ�����
	 *
	 * ˵����
	 *       ��Filter�г��ֵ��ַ����ɿͻ�������������
	 *       ���ұ�֤��Filter������������Ч
	 *       ���еĹ��������������������ڣ����鶼��ջ�ϴ���
	 *       ���磺enter("role", &YCValueFilter("knuth", &YCAttrFilter("sex", "male", NULL)))
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
		// ������apply(void *doc, void * newElement)
		//
		// Ŀ�ģ����½ڵ㲹�����������
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
		// ������match(void *node)
		//
		// Ŀ�ģ��ڵ��Ƿ�ƥ������
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
		// ������selfApply(void *doc, void * newElement)
		//
		// Ŀ�ģ����½ڵ㲹�����������
		//
		virtual void selfApply(void *doc, void * newElement) = 0;

		//
		// ������selfMatch(void *node)
		//
		// Ŀ�ģ�����ƥ��
		//
		virtual bool selfMatch(void *node) = 0;

		//
		// ���ԣ�����������
		//
		YCIXmlFilter *myNext;
	};

	/*
	 * YCNameFilter : ���ƹ�����
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
		// ������selfApply(void *doc, void * newElement)
		//
		// Ŀ�ģ����½ڵ㲹�����������
		//
		virtual void selfApply(void *doc, void * newElement);

		//
		// ������selfMatch(void *node)
		//
		// Ŀ�ģ�����ƥ��
		//
		virtual bool selfMatch(void *node);

	private:

		const char* myName;
	};

	/*
	 * YCAttrFilter : ���Թ�����
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
		// ������selfApply(void *doc, void * newElement)
		//
		// Ŀ�ģ����½ڵ㲹�����������
		//
		virtual void selfApply(void *doc, void * newElement);

		//
		// ������selfMatch(void *node)
		//
		// Ŀ�ģ�����ƥ��
		//
		virtual bool selfMatch(void *node);

	private:

		const char* myName;
		const char* myValue;
	};

	/*
	 * YCValueFilter : ֵ������
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
		// ������selfApply(void *doc, void * newElement)
		//
		// Ŀ�ģ����½ڵ㲹�����������
		//
		virtual void selfApply(void *doc, void * newElement);

		//
		// ������selfMatch(void *node)
		//
		// Ŀ�ģ�����ƥ��
		//
		virtual bool selfMatch(void *node);

	private:

		const char* myValue;
	};

public:
	
	YCXmlWriter(const char* filename);
	
	~YCXmlWriter(void);

	//
	// ������validate()
	//
	// Ŀ�ģ�У��XML�ĵ���������
	//
	bool validate();

	// 
	// ������enter(const char* name, YCIXmlFilter* filter)
	//
	// Ŀ�ģ������ӽڵ�
	//
	YCXmlWriter& enter(const char* name, YCIXmlFilter* filter);

	//
	// ������foreach(const char* name, YCIXmlFilter* filter, YCIForEachWrapper& wrapper)
	//
	// Ŀ�ģ�������Ϊnode���ӽڵ�
	//
	YCXmlWriter& foreach(const char* name, YCIXmlFilter* filter, YCIForEachWrapper& wrapper);

	//
	// ������value(const char* val)
	//
	// Ŀ�ģ�д�ַ���ֵ
	//
	YCXmlWriter& value(const char* val);

	//
	// ������value(int& val)
	//
	// Ŀ�ģ�дint��ֵ
	//
	YCXmlWriter& value(int val);
	
	//
	// ������attr(const char *name, const char* str)
	//
	// Ŀ�ģ�д�ַ�������ֵ
	//
	YCXmlWriter& attr(const char *name, const char* val);

	//
	// ������attr(const char *name, int str)
	//
	// Ŀ�ģ�дint������ֵ
	//
	YCXmlWriter& attr(const char *name, int val);

	//
	// ������append(const char *name, const char* val)
	//
	// Ŀ�ģ�׷�ӽڵ�
	//
	YCXmlWriter& append(const char *name, const char* val);

	//
	// ������append(const char *name, int val)
	//
	// Ŀ�ģ�׷�ӽڵ�
	//
	YCXmlWriter& append(const char *name, int val);

	//
	// ������removeAttr(const char* name)
	//
	// Ŀ�ģ��Ƴ���Ϊname������
	//
	YCXmlWriter& removeAttr(const char* name);

	//
	// ������remove(YCIXmlFilter* filter)
	//
	// Ŀ�ģ��Ƴ�filterΪtrue���ӽڵ�
	//       filterΪNULL���Ƴ������ӽڵ�
	//
	YCXmlWriter& remove(YCIXmlFilter* filter);
	
	//
	// ������leave()
	//
	// Ŀ�ģ��ص����ڵ�
	//
	YCXmlWriter& leave();

	//
	// ������flush()
	//
	// Ŀ�ģ��־û��ļ�
	//
	void flush();

private:

	//
	// ���ԣ��ļ���
	//
	char * myFilename;

	//
	// ���ԣ�д�����ڲ�ʵ����
	//
	YCXmlWriterImpl *myWriterImpl;
};

#endif