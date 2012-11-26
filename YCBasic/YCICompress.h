#ifndef _INCLUDE_YCICOMPRESS_H_
#define _INCLUDE_YCICOMPRESS_H_

/*
 * YCICompress : ѹ���ӿ���
 */
class YCAPI YCICompress
{
public:

	typedef bool (*COMPRESS_CALLBACK)(const char* out, unsigned int len);
	
public:

	YCICompress(void);

	virtual ~YCICompress(void);

	///////////////////////////////////////////////////////////////////////
	// compress
	///////////////////////////////////////////////////////////////////////

	//
	// ˵��������ѹ�����������ؾ�ֵ
	//
	virtual unsigned int setProperty(unsigned int name, unsigned int value) = 0;

	//
	// ˵���� ͨ���ص�������ƣ��ͻ����ù����ڴ�ϸ��     
	//
	//       in	 : ��ѹ������
	//       len : ��ѹ������
	//       cb  : ѹ���ɹ��ص�
	//
	virtual bool compress(const char* in, unsigned int len, COMPRESS_CALLBACK cb) = 0;

	//
	// ˵�����ͻ����䣬�ͷ��ڴ� ������Ȳ��������׳��쳣
	//
	//       in	  : ��ѹ������
	//       ilen : ��ѹ������
	//       out  : ѹ��������
	//       olen : ѹ��������
	//
	virtual bool compress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen) = 0;

	//
	// ˵���� �ڴ��ڲ����䣬�ͻ��ͷ��ڴ�
	//
	//       in	  : ��ѹ������
	//       ilen : ��ѹ������
	//       out  : ѹ��������
	//       olen : ѹ��������
	//
	virtual bool compress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen) = 0;

	//
	// ˵����ȡ��ѹ����ĳ��ȣ����ڿͻ������ڴ�
	//
	virtual unsigned int bufferForCompress(unsigned int len) = 0;

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
	virtual bool uncompress(const char* src, unsigned int len, COMPRESS_CALLBACK cb) = 0;

	//
	// ˵���� �ͻ��Լ����䣬�����ڴ�ϸ��     
	//
	//       in	  : ����ѹ������
	//       ilen : ����ѹ������
	//       out  : ��ѹ��������
	//       olen : ��ѹ��������
	//
	virtual bool uncompress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen) = 0;

	//
	// ˵���� �ڴ��ڲ����䣬�ͻ��ͷ��ڴ�     
	//
	//       in	  : ����ѹ������
	//       ilen : ����ѹ������
	//       out  : ��ѹ��������
	//       olen : ��ѹ��������
	//
	virtual bool uncompress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen) = 0;


};

#endif

