#ifndef _INCLUDE_YCIPACKPERSIST_H_
#define _INCLUDE_YCIPACKPERSIST_H_

//
// YCILYCPersist : 接口类 读写文件的处理，压解缩，加解密
//
//    返回成功：
//         out : 内存在内部分配，客户管理释放 SAFE_DELETE_ARRAY
//    
//    返回失败：
//         out : 不带内存
//
class YCIPackPersist
{
public:

	virtual ~YCIPackPersist() {}

	//
	// 写入文件前的处理，加密，压缩等
	//
	virtual bool encode(const char* in, unsigned int ilen, char** out, unsigned int& olen) = 0;

	//
	// 读出文件的处理，解压，解密等
	//
	virtual bool decode(const char* in, unsigned int ilen, char** out, unsigned int& olen) = 0;

};

#endif

