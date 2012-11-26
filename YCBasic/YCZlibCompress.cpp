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
// 说明：设置压缩参数，返回旧值
//
unsigned int YCZlibCompress::setProperty(unsigned int name, unsigned int value)
{
	if (name != COMPRESS_LEVEL)
	{
		throw YCException(1002, "YCZlibCompress::setProperty不支持的参数", name);
	}

	unsigned int oldLevel = myCompressLevel;
	myCompressLevel = value;
	return oldLevel;
}

//
// 说明： 通过回调函数设计，客户不用关心内存细节     
//
//       in	 : 待压缩内容
//       len : 待压缩长度
//       cb  : 压缩成功回调
//
bool YCZlibCompress::compress(const char* src, unsigned int len, COMPRESS_CALLBACK cb)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(len > 0);
	SMART_ASSERT(cb != NULL);
	if ((src == NULL) || len == 0)
	{
		throw YCException(1002, "YCZlibCompress::compress压缩数据有误");
	}

	if (cb == NULL)
	{
		throw YCException(1002, "YCZlibCompress::compress未提供回调函数");
	}

	unsigned int outLen = bufferForCompress(len);
	if (outLen == 0)
	{
		throw YCException(1002, "YCZlibCompress::compress压缩数据有误, 无法计算压缩长度");
	}
	char* dst = new char[outLen];
	int result = compress2((Bytef*)dst, (uLongf*)&outLen, (Bytef*)src, len, myCompressLevel);
	if (result != Z_OK)
	{
		throw YCException(1002, "YCZlibCompress::compress压缩数据错误：", result);
	}

	try 
	{
		if (!cb(dst, outLen))
		{
			LOG_WARNING("YCZlibCompress::compress回调函数返回false");
		}
	}
	catch (YCException& e)
	{
		LOG_ERROR("YCZlibCompress::compress回调函数异常!" << e.what());
		return false;
	}

	SAFE_DELETE_ARRAY(dst);
	return true;
}

//
// 说明：客户分配，释放内存 如果长度不够，将抛出异常
//
//       in	  : 待压缩内容
//       ilen : 待压缩长度
//       out  : 压缩出内容
//       olen : 压缩出长度
//
bool YCZlibCompress::compress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(inLen > 0);
	SMART_ASSERT(dst != NULL);
	SMART_ASSERT(outLen > 0);
	if ((src == NULL) || (inLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::compress压缩IN数据有误");
	}

	if ((dst == NULL) || (outLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::compress压缩OUT数据有误");
	}

	return (Z_OK == compress2((Bytef*)dst, (uLongf*)&outLen, (Bytef*)src, inLen, myCompressLevel));
}

//
// 说明： 客户自己分配，管理内存细节     
//
//       in	  : 待压缩内容
//       ilen : 待压缩长度
//       out  : 压缩出内容
//       olen : 压缩出长度
//
bool YCZlibCompress::compress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(inLen > 0);
	SMART_ASSERT(dst != NULL);

	if ((src == NULL) || (inLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::compress压缩IN数据有误");
	}
	
	if ((dst == NULL))
	{
		throw YCException(1002, "YCZlibCompress::compress压缩OUT数据有误");
	}

	const unsigned int MaxLen = 10000000;
	unsigned char* _zipDst = new unsigned char[MaxLen];
	SCOPE_GUARD(zip_guard, delete[] _zipDst);

	//先对原始内容进行压缩工作
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
		throw YCException(1002, "YCZlibCompress::compress压缩文件deflateInit2出错", ret);
	}

	ret = deflate(&c_stream, Z_FINISH);
	if (ret != Z_STREAM_END)
	{
		deflateEnd(&c_stream);
		throw YCException(1002, "YCZlibCompress::compress压缩文件deflate出错", ret);
	}

	outLen = c_stream.total_out;
	deflateEnd(&c_stream);

	//压缩完毕进行返回包组织
	*dst = new char[outLen];
	memcpy(*dst, _zipDst, outLen);

	return true;
}

//
// 说明：取得压缩后的长度，便于客户分配内存
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
// 说明： 通过回调函数设计，客户不用关心内存细节     
//
//       in	 : 待解压缩内容
//       len : 待解压缩长度
//       cb  : 解压缩成功回调
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
		throw YCException(1002, "YCZlibCompress::uncompress解压缩inflateInit2错误", ret); 
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
			throw YCException(1002, "YCZlibCompress::uncompress解压缩inflate错误", ret);
		}

		have = CHUNK - strm.avail_out; 
		outLen += have; 
		dst = (char*)realloc(dst, outLen); 
		memcpy(dst+outLen-have, buf, have); 
	} while (strm.avail_out == 0); 

	inflateEnd(&strm); /* clean up and return */ 
	if (ret != Z_STREAM_END)
	{
		throw YCException(1002, "YCZlibCompress::uncompress解压缩inflateEnd错误", ret);
	}

	try 
	{
		if (!cb(dst, outLen))
		{
			LOG_WARNING("YCZlibCompress::compress回调函数返回false");
		}
	}
	catch (YCException& e)
	{
		LOG_ERROR("YCZlibCompress::compress回调函数异常!" << e.what());
		return false;
	}

	return true; 	
}

//
// 说明： 客户自己分配，管理内存细节     
//
//       in	  : 待解压缩内容
//       ilen : 待解压缩长度
//       out  : 解压缩出内容
//       olen : 解压缩出长度
//
bool YCZlibCompress::uncompress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(inLen > 0);
	SMART_ASSERT(dst != NULL);
	SMART_ASSERT(outLen > 0);
	if ((src == NULL) || (inLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::uncompress压缩IN数据有误");
	}

	if ((dst == NULL) || (outLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::uncompress压缩OUT数据有误");
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
		throw YCException(1002, "YCZlibCompress::uncompress解压缩inflateInit2错误", ret); 
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
			throw YCException(1002, "YCZlibCompress::uncompress解压缩inflate错误", ret);
		}

		unsigned int have = CHUNK - strm.avail_out; 
		outLen += have;
		memcpy(dst+outLen-have, buf, have); 
	} while (strm.avail_out == 0); 

	ret = inflateEnd(&strm);
	if (ret != Z_OK && ret != Z_STREAM_END)
	{
		throw YCException(1002, "YCZlibCompress::uncompress解压缩inflateEnd错误", ret);
	}

	return true; 	
}

//
// 说明： 客户自己分配，管理内存细节     
//
//       in	  : 待解压缩内容
//       ilen : 待解压缩长度
//       out  : 解压缩出内容
//       olen : 解压缩出长度
//
bool YCZlibCompress::uncompress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen)
{
	SMART_ASSERT(src != NULL);
	SMART_ASSERT(inLen > 0);
	SMART_ASSERT(dst != NULL);
	SMART_ASSERT(*dst == NULL);
	if ((src == NULL) || (inLen == 0))
	{
		throw YCException(1002, "YCZlibCompress::uncompress压缩IN数据有误");
	}

	if ((dst == NULL) || (*dst != NULL))
	{
		throw YCException(1002, "YCZlibCompress::uncompress压缩OUT数据有误");
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
		throw YCException(1002, "YCZlibCompress::uncompress解压缩inflateInit2错误", ret); 
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
			throw YCException(1002, "YCZlibCompress::uncompress解压缩inflate错误", ret);
		}

		unsigned int have = CHUNK - strm.avail_out; 
		outLen += have;
		*dst = (char*)realloc(*dst, outLen);
		memcpy((*dst)+outLen-have, buf, have); 
	} while (strm.avail_out == 0); 

	ret = inflateEnd(&strm);
	if (ret != Z_OK && ret != Z_STREAM_END)
	{
		throw YCException(1002, "YCZlibCompress::uncompress解压缩inflateEnd错误", ret);
	}

	return true; 	
}
