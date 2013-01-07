#ifndef _INCLUDE_YCTEST_H_
#define _INCLUDE_YCTEST_H_

struct YCTest
{
public:

	static void test();

private:

	//
	// 测试MD5计算
	//
	static void test_md5();

	//
	// 测试接收NPC聊天消息
	//
	static void test_npctalk();
	//
	// 测试方法转发
	//
	static void test_forward();

	//
	// 打包文件，创建打开测试
	//
	static void test_packfile();

	//
	// 加解密测试
	//
	static void test_encrypt();

	//
	// 加解密文件测试
	//
	static void test_encrypt_file();

	//
	// 内部分配内存测试
	//
	static void test_compress();

	//
	// 外部分配内存测试
	//
	static void test_compress2();
};

#endif