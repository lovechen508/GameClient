#ifndef _INCLUDE_YCNLS_H_
#define _INCLUDE_YCNLS_H_

#include <string>
#include <map>

#include <YCBasic\YCDef.h>

/*
 * YCNls : Native Language Support
 *
 *   ˵��: 
 *        1. �ļ�����֧�ֵ�������Ϊ��չ��
 *              �������� *.cn_zh
 *              Ӣ��     *.en
 *
 *        2. ���ø�ʽ
 *             level := �ȼ�
 */
class YCNls
{
public:

	// ֧�ֵ������б�
	enum E_I18N_LANG{
		CN_ZH          = 1,
		EN             = 2
	};

	//
	// ������
	//
	// Ŀ�ģ�
	//
	YCNls(const char* filename);

	//
	// ������
	//
	// Ŀ�ģ�
	//
	~YCNls(void);

	//
	// ������initialize()
	//
	// Ŀ�ģ���ȡNLS�����ļ�
	//
	bool initialize();

	//
	// ������lookup(const char* key)
	//
	// Ŀ�ģ�����NLS����
	//
	const char* lookup(const char* key);

private:

	char myNlsFile[MAX_PATH];

	std::map<const char*, const char*> myNlsMap;
};

#endif
