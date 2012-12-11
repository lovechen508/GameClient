#include "stdafx.h"
#include "YCTest.h"

#include "YCGame\YCTaskParser.h"
#include "YCGame\YCTaskMessage.h"
#include "YCNetwork\protocol\YCPkg_0090_Npc.h"

#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCScopeGuard.h"
#include "YCBasic\YCFuncAdapter.h"
#include "YCBasic\YCZlibCompress.h"
#include "YCBasic\YCBFEncryption.h"

#include "YCFile\YCLYCPackSystem.h"

void YCTest::test()
{
	test_npctalk();
	//test_forward();
	//test_packfile();
	//test_encrypt();
	//test_compress();
	//test_compress2();
}

//
// ���Խ���NPC������Ϣ
//
void YCTest::test_npctalk()
{/*
	NET_TASK_MSG_NTF_T ntf;
	const char* testMsg = "[start]\r\n\
		talk =\"<?xml version='1.0' encoding='GB2312'?><p><t>éɽ������</t><t>\r\n\r\n</t><t color='ffff0000'>���鳾�����������������ٵ�ʱ��˭���޷����ܣ�</t></p>\"\r\n\
		1 = 'link' '�ָ��' 'dlg1'\r\n\
		2 = 'server_cmd' '���ͻ��ͼ' '1'\r\n\
		3 = 'link' '��ɱ���ѯ' 'dlg2'\r\n\
		4 = 'link' '��ȡ�����' 'dlg3'\r\n\
		5 = 'server_cmd' '�뿪' '2'\r\n\
		[dlg1]\r\n\
		talk =\"�ָ��\"\r\n\
		1 = 'link' '�' 'dlg11'\r\n\
		2 = 'link' '���񿼺���' 'dlg12'\r\n\
		3 = 'link' '����' 'start'\r\n\
		                  ";

	unsigned int length = strlen(testMsg);
	ntf.count = length;
	memcpy(ntf.message, testMsg, length);

	YCTaskMessage* result = YCTaskParser::parse(&ntf);
	if (result == NULL)
	{
		throw YCException(102, "YCTest::test_npctalk����ʧ��");
	}
    */
}

//
// ���Է���ת��
//
void YCTest::test_forward()
{

}

void YCTest::test_packfile()
{
	YCLYCPackSystem fileSystem;
	fileSystem.initialize();
	if (!YCFileUtil::IsFileExist("c:\\PACK\\liao.lyc"))
	{
		fileSystem.create("c:\\PACK\\liao.lyc", 1);
	}
	else
	{
		fileSystem.mount("c:\\PACK");
	}

	if (!fileSystem.find("liao.xml", "\\liao\\zan"))
	{
		fileSystem.copy("c:\\liao.xml", "\\liao\\zan", true);
	}
	else
	{
		//fileSystem.dump("\\liao\\zan\\liao.xml", "c:\\dump", true);
		fileSystem.erase("\\liao\\zan\\liao.xml");
	}
	
	if (!YCFileUtil::IsFileExist("c:\\PACK\\ui.lyc"))
	{
		fileSystem.create("c:\\PACK\\ui.lyc", 1);
		fileSystem.mkdir("\\ui\\1\\layout", true);
		//fileSystem.copy("D:\\dropbox\\Dropbox\\Public\\Client\\data\\ui", "\\ui", true);
		fileSystem.dump("\\ui", "c:\\dump", true);
	}
	else
	{
		fileSystem.mount("c:\\PACK");
	}

	for (int i = 0; i < 2; ++i)
	{
		if (fileSystem.find("layout", "\\ui\\1\\"))
		{
			fileSystem.erase("\\ui\\1\\layout");
		}
		else
		{
			fileSystem.mkdir("\\ul\\1\\layout", false);
			fileSystem.copy("D:\\dropbox\\Dropbox\\Public\\Client\\data\\ui\\1\\layout", "\\ui\\1\\layout", true);
		}
	}
	

	fileSystem.finalize();
}

void YCTest::test_encrypt()
{
	const char* pwd = "<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s";

	char* src = new char[MAX_PATH];
	char* out = new char[MAX_PATH];
	char* tmp = new char[MAX_PATH];

	SCOPE_GUARD(src_guard, delete[] src);
	SCOPE_GUARD(tmp_guard, delete[] tmp);
	SCOPE_GUARD(out_guard, delete[] out);

	for (int i = 0; i < MAX_PATH; ++i)
	{
		src[i] = i;
	}

	unsigned int outlen = MAX_PATH;
	YCBFEncryption bf(pwd);
	
	if (!bf.encode(src, strlen(pwd), tmp, outlen))
	{
		throw YCException(902, "YCTest::test_encrypt encodeʧ��");
	}

	if (!bf.decode(tmp, outlen, out, outlen))
	{
		throw YCException(902, "YCTest::test_encrypt decode ʧ��");
	}

	for (int i = 0; i < strlen(pwd); ++i)
	{
		if (src[i] != out[i])
		{
			throw YCException(902, "YCTest::test_encrypt �ӽ��ܲ�һ��");
		}
	}
}

//
// �ڲ������ڴ����
//
void YCTest::test_compress()
{
	std::string src("<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s");
	YCZlibCompress comp;

	char* tmp = NULL;

	unsigned int oLen;

	if (!comp.compress(src.c_str(), src.length(), &tmp, oLen))
	{
		throw YCException(902, "YCTest::test_compressѹ��ʧ��");
	}
	
	char* dst = NULL;
	if (!comp.uncompress(tmp, oLen, &dst, oLen))
	{
		throw YCException(902, "YCTest::test_compress��ѹʧ��");
	}

	if (oLen != src.length())
	{
		throw YCException(902, "YCTest::test_compress�ӽ�ѹ���Ȳ�һ��");
	}

	for (int i = 0; i < src.length(); ++i)
	{
		if (src[i] != dst[i])
		{
			throw YCException(902, "YCTest::test_compress�ӽ�ѹ���ݲ�һ��");
		}
	}
}

//
// �ⲿ�����ڴ����
//
void YCTest::test_compress2()
{
	std::string src("<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s");
	YCZlibCompress comp;

	//char* tmp = NULL;
	char* tmp = new char[1024];
	
	unsigned int oLen;

	if (!comp.compress(src.c_str(), src.length(), tmp, oLen))
	{
		throw YCException(902, "YCTest::test_compressѹ��ʧ��");
	}

	char* dst = new char[1024];
	if (!comp.uncompress(tmp, oLen, dst, oLen))
	{
		throw YCException(902, "YCTest::test_compress��ѹʧ��");
	}

	if (oLen != src.length())
	{
		throw YCException(902, "YCTest::test_compress�ӽ�ѹ���Ȳ�һ��");
	}

	for (int i = 0; i < src.length(); ++i)
	{
		if (src[i] != dst[i])
		{
			throw YCException(902, "YCTest::test_compress�ӽ�ѹ���ݲ�һ��");
		}
	}
}