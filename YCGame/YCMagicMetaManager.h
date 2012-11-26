#ifndef _INCLUDE_YCMAGICMETAMANAGER_H_
#define _INCLUDE_YCMAGICMETAMANAGER_H_

#include <map>

class YCMagicMeta;

/*
 * YCMagicMetaManager : ����Ԫ���ݹ�����
 */
class YCMagicMetaManager 
{
public:

	YCMagicMetaManager(void);
	
	~YCMagicMetaManager(void);

	//
	// ������setMetaInfo(YCMagicMeta* magicMeta)
	//
	// Ŀ�ģ����Ӽ���Ԫ���ݶ���
	//
	void setMetaInfo(YCMagicMeta* magicMeta);
	//
	// ������getMetaInfo(unsigned int magicId)
	//
	// Ŀ�ģ���ȡ����Ԫ���ݶ���
	//
	YCMagicMeta* getMetaInfo(unsigned int magicId);

private:

	//
	// ����Ԫ����ӳ��
	//
	std::map<unsigned int, YCMagicMeta*> myMagicMetas; 
};

#endif

