#ifndef _INCLUDE_YCICOMPRESS_H_
#define _INCLUDE_YCICOMPRESS_H_

/*
 * YCICompress : 压缩接口类
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
	// 说明：设置压缩参数，返回旧值
	//
	virtual unsigned int setProperty(unsigned int name, unsigned int value) = 0;

	//
	// 说明： 通过回调函数设计，客户不用关心内存细节     
	//
	//       in	 : 待压缩内容
	//       len : 待压缩长度
	//       cb  : 压缩成功回调
	//
	virtual bool compress(const char* in, unsigned int len, COMPRESS_CALLBACK cb) = 0;

	//
	// 说明：客户分配，释放内存 如果长度不够，将抛出异常
	//
	//       in	  : 待压缩内容
	//       ilen : 待压缩长度
	//       out  : 压缩出内容
	//       olen : 压缩出长度
	//
	virtual bool compress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen) = 0;

	//
	// 说明： 内存内部分配，客户释放内存
	//
	//       in	  : 待压缩内容
	//       ilen : 待压缩长度
	//       out  : 压缩出内容
	//       olen : 压缩出长度
	//
	virtual bool compress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen) = 0;

	//
	// 说明：取得压缩后的长度，便于客户分配内存
	//
	virtual unsigned int bufferForCompress(unsigned int len) = 0;

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
	virtual bool uncompress(const char* src, unsigned int len, COMPRESS_CALLBACK cb) = 0;

	//
	// 说明： 客户自己分配，管理内存细节     
	//
	//       in	  : 待解压缩内容
	//       ilen : 待解压缩长度
	//       out  : 解压缩出内容
	//       olen : 解压缩出长度
	//
	virtual bool uncompress(const char* src, unsigned int inLen, char* dst, unsigned int& outLen) = 0;

	//
	// 说明： 内存内部分配，客户释放内存     
	//
	//       in	  : 待解压缩内容
	//       ilen : 待解压缩长度
	//       out  : 解压缩出内容
	//       olen : 解压缩出长度
	//
	virtual bool uncompress(const char* src, unsigned int inLen, char** dst, unsigned int& outLen) = 0;


};

#endif

