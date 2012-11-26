#ifndef _INCLUDE_YCZLIBCOMPRESS_H_
#define _INCLUDE_YCZLIBCOMPRESS_H_

#include "YCICompress.h"

class YCAPI YCZlibCompress : public YCICompress
{
public:

	enum ZLIB_PROPERTY {
		COMPRESS_LEVEL = 1,
	};

public:

	YCZlibCompress(void);

	virtual ~YCZlibCompress(void);

	///////////////////////////////////////////////////////////////////////
	// compress
	///////////////////////////////////////////////////////////////////////

	//
	// ˵��������ѹ�����������ؾ�ֵ
	//
	unsigned int setProperty(unsigned int name, unsigned int value);

	//
	// ˵���� ͨ���ص�������ƣ��ͻ����ù����ڴ�ϸ��     
	//
	//       in	 : ��ѹ������
	//       len : ��ѹ������
	//       cb  : ѹ���ɹ��ص�
	//
	virtual bool compress(const char* src, unsigned int len, COMPRESS_CALLBACK cb);

	//
	// ˵�����ͻ����䣬�ͷ��ڴ� ������Ȳ��������׳��쳣
	//
	//       in	  : ��ѹ������
	//       ilen : ��ѹ������
	//       out  : ѹ��������
	//       olen : ѹ��������
	//
	virtual bool compress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen);

	//
	// ˵���� �ڴ��ڲ����䣬�ͻ��ͷ��ڴ�
	//
	//       in	  : ��ѹ������
	//       ilen : ��ѹ������
	//       out  : ѹ��������
	//       olen : ѹ��������
	//
	virtual bool compress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen);

	//
	// ˵����ȡ��ѹ����ĳ��ȣ����ڿͻ������ڴ�
	//
	virtual unsigned int bufferForCompress(unsigned int len);

	/////////////////////////////////////////////////////////////////////
	// uncompress
	/////////////////////////////////////////////////////////////////////

	//
	// ˵���� ͨ���ص�������ƣ��ͻ����ù����ڴ�ϸ��     
	//
	//       in	 : ����ѹ������
	//       len : ����ѹ������
	//       cb  : ��ѹ���ɹ��ص�
	//
	virtual bool uncompress(const char* src, unsigned int len, COMPRESS_CALLBACK cb);

	//
	// ˵���� �ͻ��Լ����䣬�����ڴ�ϸ��     
	//
	//       in	  : ����ѹ������
	//       ilen : ����ѹ������
	//       out  : ��ѹ��������
	//       olen : ��ѹ��������
	//
	virtual bool uncompress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen);

	//
	// ˵���� �ڴ��ڲ����䣬�ͻ��ͷ��ڴ�     
	//
	//       in	  : ����ѹ������
	//       ilen : ����ѹ������
	//       out  : ��ѹ��������
	//       olen : ��ѹ��������
	//
	virtual bool uncompress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen);

private:

	//
	// ѹ������0-9
	//
	unsigned int myCompressLevel;
};

#endif

