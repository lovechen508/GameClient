#ifndef _INCLUDE_YCTEST_H_
#define _INCLUDE_YCTEST_H_

struct YCTest
{
public:

	static void test();

private:

	//
	// ����MD5����
	//
	static void test_md5();

	//
	// ���Խ���NPC������Ϣ
	//
	static void test_npctalk();
	//
	// ���Է���ת��
	//
	static void test_forward();

	//
	// ����ļ��������򿪲���
	//
	static void test_packfile();

	//
	// �ӽ��ܲ���
	//
	static void test_encrypt();

	//
	// �ӽ����ļ�����
	//
	static void test_encrypt_file();

	//
	// �ڲ������ڴ����
	//
	static void test_compress();

	//
	// �ⲿ�����ڴ����
	//
	static void test_compress2();
};

#endif