#include "YCZlibCompress.h"

#define ZLIB_WINAPI
#include "zlib.h"

#include "YCLogger.h"
#include "YCAssert.h"
#include "YCException.h"
#include "YCScopeGuard.h"

#define CHUNK 100000

YCZlibCompress::YCZlibCompress(void)
	: myCompressLevel(Z_DEFAULT_COMPRESSION)
{
}


YCZlibCompress::~YCZlibCompress(void)
{
}

///////////////////////////////////////////////////////////////////////
// compress
///////////////////////////////////////////////////////////////////////

//
// ˵��������ѹ�����������ؾ�ֵ
//
unsigned int YCZlibCompress::setProperty(unsigned int name, unsigned int value)
{
	if (name != COMPRESS_LEVEL)
	{
		throw YCException(1002, "YCZlibCompress::setProperty��֧�ֵĲ���", name);
	}

	unsigned int oldLevel = myCompressLevel;
	myCompressLevel = value;
	return oldLevel;
}

//
// ˵���� ͨ���ص�������ƣ��ͻ����ù����ڴ�ϸ��     
//
//       in	 : ��ѹ������
//       len : ��ѹ������
//       cb  : ѹ���ɹ��ص�
//
bool YCZlibCompress::compress(const char* src, unsigned int len, COMPRESS_CALLBACK cb)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(len > 0);
	SMART_ASSERT(cb != NULL);
	if ((src == NULL) || len == 0)
	{
		throw YCException(1002, "YCZlibCompress::compressѹ����������");
	}

	if (cb == NULL)
	{
		throw YCException(1002, "YCZlibCompress::compressδ�ṩ�ص�����");
	}

	unsigned int outLen = bufferForCompress(len);
	if (outLen == 0)
	{
		throw YCException(1002, "YCZlibCompress::compressѹ����������, �޷�����ѹ������");
	}
	char* dst = new char[outLen];
	int result = compress2((Bytef*)dst, (uLongf*)&outLen, (Bytef*)src, len, myCompressLevel);
	if (result != Z_OK)
	{
		throw YCException(1002, "YCZlibCompress::compressѹ�����ݴ���", result);
	}

	try 
	{
		if (!cb(dst, outLen))
		{
			LOG_WARNING("YCZlibCompress::compress�ص���������false");
		}
	}
	catch (YCException& e)
	{
		LOG_ERROR("YCZlibCompress::compress�ص������쳣!" << e.what());
		return false;
	}

	SAFE_DELETE_ARRAY(dst);
	return true;
}

//
// ˵�����ͻ����䣬�ͷ��ڴ� ������Ȳ��������׳��쳣
//
//       in	  : ��ѹ������
//       ilen : ��ѹ������
//       out  : ѹ��������
//       olen : ѹ��������
//
bool YCZlibCompress::compress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(inLen > 0);
	SMART_ASSERT(dst != NULL);
	SMART_ASSERT(outLen > 0);
	if ((src == NULL) || (inLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::compressѹ��IN��������");
	}

	if ((dst == NULL) || (outLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::compressѹ��OUT��������");
	}

	return (Z_OK == compress2((Bytef*)dst, (uLongf*)&outLen, (Bytef*)src, inLen, myCompressLevel));
}

//
// ˵���� �ͻ��Լ����䣬�����ڴ�ϸ��     
//
//       in	  : ��ѹ������
//       ilen : ��ѹ������
//       out  : ѹ��������
//       olen : ѹ��������
//
bool YCZlibCompress::compress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(inLen > 0);
	SMART_ASSERT(dst != NULL);

	if ((src == NULL) || (inLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::compressѹ��IN��������");
	}
	
	if ((dst == NULL))
	{
		throw YCException(1002, "YCZlibCompress::compressѹ��OUT��������");
	}

	const unsigned int MaxLen = 10000000;
	unsigned char* _zipDst = new unsigned char[MaxLen];
	SCOPE_GUARD(zip_guard, delete[] _zipDst);

	//�ȶ�ԭʼ���ݽ���ѹ������
	z_stream c_stream;
	c_stream.zalloc = Z_NULL;
	c_stream.zfree = Z_NULL;
	c_stream.opaque = Z_NULL;
	c_stream.next_in = (Bytef*)src;
	c_stream.avail_in = inLen;
	c_stream.next_out = (Bytef*)_zipDst;
	c_stream.avail_out = (unsigned int)MaxLen;

	//int ret = deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION,Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
	int ret = deflateInit(&c_stream, myCompressLevel);
	if(ret != Z_OK)
	{
		throw YCException(1002, "YCZlibCompress::compressѹ���ļ�deflateInit2����", ret);
	}

	ret = deflate(&c_stream, Z_FINISH);
	if (ret != Z_STREAM_END)
	{
		deflateEnd(&c_stream);
		throw YCException(1002, "YCZlibCompress::compressѹ���ļ�deflate����", ret);
	}

	outLen = c_stream.total_out;
	deflateEnd(&c_stream);

	//ѹ����Ͻ��з��ذ���֯
	*dst = new char[outLen];
	memcpy(*dst, _zipDst, outLen);

	return true;
}

//
// ˵����ȡ��ѹ����ĳ��ȣ����ڿͻ������ڴ�
//       must be at least 0.1% larger than sourceLen plus 12 bytes.
//
unsigned int YCZlibCompress::bufferForCompress(unsigned int len)
{
	return compressBound(len);
}

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
bool YCZlibCompress::uncompress(const char* src, unsigned int len, COMPRESS_CALLBACK cb)
{
	unsigned have; 
	z_stream strm; 
	unsigned char buf[CHUNK]; 
	int outLen = 0; 

	/* allocate inflate state */ 
	strm.zalloc = Z_NULL; 
	strm.zfree = Z_NULL; 
	strm.opaque = Z_NULL; 
	strm.avail_in = 0; 
	strm.next_in = Z_NULL; 

	int ret = inflateInit2(&strm, 31); 
	if (ret != Z_OK) 
	{
		throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflateInit2����", ret); 
	}

	char* dst = NULL;
	strm.avail_in = len; 
	strm.next_in = (unsigned char*)src; 

	/* run inflate() on input until output buffer not full */ 
	do { 
		strm.avail_out = CHUNK; 
		strm.next_out = buf; 
		ret = inflate(&strm, Z_NO_FLUSH); 
		if (ret != Z_OK) 
		{ 
			inflateEnd(&strm); 
			throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflate����", ret);
		}

		have = CHUNK - strm.avail_out; 
		outLen += have; 
		dst = (char*)realloc(dst, outLen); 
		memcpy(dst+outLen-have, buf, have); 
	} while (strm.avail_out == 0); 

	inflateEnd(&strm); /* clean up and return */ 
	if (ret != Z_STREAM_END)
	{
		throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflateEnd����", ret);
	}

	try 
	{
		if (!cb(dst, outLen))
		{
			LOG_WARNING("YCZlibCompress::compress�ص���������false");
		}
	}
	catch (YCException& e)
	{
		LOG_ERROR("YCZlibCompress::compress�ص������쳣!" << e.what());
		return false;
	}

	return true; 	
}

//
// ˵���� �ͻ��Լ����䣬�����ڴ�ϸ��     
//
//       in	  : ����ѹ������
//       ilen : ����ѹ������
//       out  : ��ѹ��������
//       olen : ��ѹ��������
//
bool YCZlibCompress::uncompress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(inLen > 0);
	SMART_ASSERT(dst != NULL);
	SMART_ASSERT(outLen > 0);
	if ((src == NULL) || (inLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::uncompressѹ��IN��������");
	}

	if ((dst == NULL) || (outLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::uncompressѹ��OUT��������");
	}

	z_stream strm; 
	//unsigned char* buf = new unsigned char[CHUNK]; 
	//SCOPE_GUARD(buf_guard, delete[] buf);
	unsigned char buf[CHUNK]; 

	/* allocate inflate state */ 
	strm.zalloc = Z_NULL; 
	strm.zfree = Z_NULL; 
	strm.opaque = Z_NULL; 
	strm.avail_in = 0; 
	strm.next_in = Z_NULL; 

	//int ret = inflateInit2(&strm, 31); 
	int ret = inflateInit(&strm);
	if (ret != Z_OK) 
	{
		throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflateInit2����", ret); 
	}

	outLen = 0;
	strm.avail_in = inLen; 
	strm.next_in = (unsigned char*)src; 

	/* run inflate() on input until output buffer not full */ 
	do { 
		strm.avail_out = CHUNK; 
		strm.next_out = buf; 
		ret = inflate(&strm, Z_NO_FLUSH); 
		if (ret != Z_OK && ret != Z_STREAM_END) 
		{ 
			inflateEnd(&strm); 
			throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflate����", ret);
		}

		unsigned int have = CHUNK - strm.avail_out; 
		outLen += have;
		memcpy(dst+outLen-have, buf, have); 
	} while (strm.avail_out == 0); 

	ret = inflateEnd(&strm);
	if (ret != Z_OK && ret != Z_STREAM_END)
	{
		throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflateEnd����", ret);
	}

	return true; 	
}

//
// ˵���� �ͻ��Լ����䣬�����ڴ�ϸ��     
//
//       in	  : ����ѹ������
//       ilen : ����ѹ������
//       out  : ��ѹ��������
//       olen : ��ѹ��������
//
bool YCZlibCompress::uncompress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(inLen > 0);
	SMART_ASSERT(dst != NULL);
	SMART_ASSERT(*dst == NULL);
	if ((src == NULL) || (inLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::uncompressѹ��IN��������");
	}

	if ((dst == NULL) || (*dst != NULL))
	{
		throw YCException(1002, "YCZlibCompress::uncompressѹ��OUT��������");
	}

	z_stream strm; 
	unsigned char buf[CHUNK]; 

	/* allocate inflate state */ 
	strm.zalloc = Z_NULL; 
	strm.zfree = Z_NULL; 
	strm.opaque = Z_NULL; 
	strm.avail_in = 0; 
	strm.next_in = Z_NULL; 

	//int ret = inflateInit2(&strm, 31); 
	int ret = inflateInit(&strm); 
	if (ret != Z_OK) 
	{
		throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflateInit2����", ret); 
	}

	outLen = 0;
	strm.avail_in = inLen; 
	strm.next_in = (unsigned char*)src; 

	/* run inflate() on input until output buffer not full */ 
	do { 
		strm.avail_out = CHUNK; 
		strm.next_out = buf; 
		ret = inflate(&strm, Z_NO_FLUSH); 
		if (ret != Z_OK && ret != Z_STREAM_END) 
		{ 
			inflateEnd(&strm); 
			throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflate����", ret);
		}

		unsigned int have = CHUNK - strm.avail_out; 
		outLen += have;
		*dst = (char*)realloc(*dst, outLen);
		memcpy((*dst)+outLen-have, buf, have); 
	} while (strm.avail_out == 0); 

	ret = inflateEnd(&strm);
	if (ret != Z_OK && ret != Z_STREAM_END)
	{
		throw YCException(1002, "YCZlibCompress::uncompress��ѹ��inflateEnd����", ret);
	}

	return true; 	
}
