#ifndef _INCLUDE_YCMAGICMETABUILDER_H_
#define _INCLUDE_YCMAGICMETABUILDER_H_

class YCMagicMetaManager;

/*
 * YCMagicMetaBuilder : ����Ԫ���ݹ�����
 */
struct YCMagicMetaBuilder
{
	//
	// ������build(const char* filename, YCMagicMetaManager* manager)
	//
	// Ŀ�ģ����������ļ���������Ԫ����
	//
	bool build(const char* filename, YCMagicMetaManager* manager);
};

#endif

