#ifndef _INCLUDE_YCTEST_H_
#define _INCLUDE_YCTEST_H_

struct YCTest
{
public:

	static void test();

private:

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
	// �ڲ������ڴ����
	//
	static void test_compress();

	//
	// �ⲿ�����ڴ����
	//
	static void test_compress2();
};

#endif