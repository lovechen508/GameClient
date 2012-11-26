#ifndef _INCLUDE_YCXMLREADER_H_
#define _INCLUDE_YCXMLREADER_H_

#include <string>
#include "YCBasic\YCAssert.h"

class YCXmlReaderImpl;

/*
 * YCXmlReader : XML �����ļ��Ķ���
 *
 * ע�ͣ���Ϊ�漰������
 *       XML�ļ�һ��Ҫ����ΪUTF-8��ʽ
 */

class YCAPI YCXmlReader 
{
private:

	//
	// YCForEachWrapper foreach functor ��װ���ӿ�
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
	// YCForEachWrapper: foreach functor ��װ��ģ����ʵ��
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
	// YCForEachWrapper1: foreach functor ��װ��ģ����ʵ��
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
	// ������YCXmlReader(const char *file)
	//
	// Ŀ�ģ����캯�������ļ�����
	//
	YCXmlReader(const char *file);

	//
	// ������YCXmlReader(const char* buffer, unsigned int length)
	//
	// Ŀ�ģ����캯�������ڴ�δ���
	//
	YCXmlReader(const char* buffer, unsigned int length);

	//
	// �������ͷ��ڲ��ṹ
	//
	~YCXmlReader(void);

	//
	// ������validate()
	//
	// Ŀ�ģ�У��XML�ĵ���������
	//
    bool validate();

    // 
	// ������enter(const char* node)
	//
	// Ŀ�ģ������ӽڵ�
	//
    YCXmlReader& enter(const char* node);
    
	//
	// ������value(char** val)
	//
	// Ŀ�ģ�ȡ�ַ���ֵ��val���ڴ����������
	//
	YCXmlReader& value(char** val);

	//
	// ������value(char** val, const char* defaultValue)
	//
	// Ŀ�ģ�ȡ�ַ���ֵ��val���ڴ����������
	//
	YCXmlReader& value(char** val, const char* defaultValue);

	//
	// ������value(int& val)
	//
	// Ŀ�ģ�ȡint��ֵ
	//
    YCXmlReader& value(int& val);

	//
	// ������value(int& val, int defaultValue)
	//
	// Ŀ�ģ�ȡint��ֵ
	//
    YCXmlReader& value(int& val, int defaultValue);

	//
	// ������value(unsigned int& val)
	//
	// Ŀ�ģ�ȡunsigned int��ֵ
	//
	YCXmlReader& value(unsigned int& val);

	//
	// ������value(unsigned int& val, unsigned int defaultValue)
	//
	// Ŀ�ģ�ȡint��ֵ
	//
	YCXmlReader& value(unsigned int& val, unsigned int defaultValue);

	//
	// ������attr(const char *name, char** str)
	//
	// Ŀ�ģ�ȡ�ַ�������ֵ��val���ڴ����������
	//
	YCXmlReader& attr(const char *name, char** val);

	//
	// ������attr(const char *name, char** str, const char* defaultValue)
	//
	// Ŀ�ģ�ȡ�ַ�������ֵ��val���ڴ����������
	//
	YCXmlReader& attr(const char *name, char** val, const char* defaultValue);

	//
	// ������attr(const char *name, int& str)
	//
	// Ŀ�ģ�ȡint������ֵ
	//
    YCXmlReader& attr(const char *name, int& val);

	//
	// ������attr(const char *name, int& str, int defaultValue)
	//
	// Ŀ�ģ�ȡint������ֵ
	//
    YCXmlReader& attr(const char *name, int& val, int defaultValue);

	//
	// ������attr(const char *name, unsigned int& str)
	//
	// Ŀ�ģ�ȡint������ֵ
	//
	YCXmlReader& attr(const char *name, unsigned int& val);

	//
	// ������attr(const char *name, unsigned int& str, unsigned int defaultValue)
	//
	// Ŀ�ģ�ȡint������ֵ
	//
	YCXmlReader& attr(const char *name, unsigned int& val, unsigned int defaultValue);

    //
	// ������foreach(const char *node, YCIForEachWrapper& wrapper)
	//
	// Ŀ�ģ�������Ϊnode���ӽڵ�
	//
    YCXmlReader& foreach(const char *node, YCIForEachWrapper& wrapper);
 
	//
	// ������leave()
	//
	// Ŀ�ģ��ص����ڵ�
	//
    YCXmlReader& leave();

private:

	const char *get_value();

private:

    char * filename;
	YCXmlReaderImpl *myReaderImpl;
};


#endif
