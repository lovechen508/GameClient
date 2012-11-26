#ifndef _INCLUDE_YCNLS_H_
#define _INCLUDE_YCNLS_H_

#include <string>
#include <map>

#include <YCBasic\YCDef.h>

/*
 * YCNls : Native Language Support
 *
 *   说明: 
 *        1. 文件名以支持的语言作为扩展名
 *              简体中文 *.cn_zh
 *              英语     *.en
 *
 *        2. 配置格式
 *             level := 等级
 */
class YCNls
{
public:

	// 支持的语言列表
	enum E_I18N_LANG{
		CN_ZH          = 1,
		EN             = 2
	};

	//
	// 函数：
	//
	// 目的：
	//
	YCNls(const char* filename);

	//
	// 函数：
	//
	// 目的：
	//
	~YCNls(void);

	//
	// 函数：initialize()
	//
	// 目的：读取NLS配置文件
	//
	bool initialize();

	//
	// 函数：lookup(const char* key)
	//
	// 目的：查找NLS名称
	//
	const char* lookup(const char* key);

private:

	char myNlsFile[MAX_PATH];

	std::map<const char*, const char*> myNlsMap;
};

#endif
