#include "YCMagicMetaManager.h"

#include "YCMagicMeta.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

YCMagicMetaManager::YCMagicMetaManager(void)
{
}


YCMagicMetaManager::~YCMagicMetaManager(void)
{
	try
	{
		for (std::map<unsigned int, YCMagicMeta*>::iterator it = myMagicMetas.begin();
			 it != myMagicMetas.end();
			 ++it)
		{
			YCMagicMeta* magic = it->second;
			SAFE_DELETE(magic);
		}
	}
	catch (...)
	{
		LOG_ERROR("YCMagicMetaManager::~YCMagicMetaManager�ͷż���Ԫ����ʧ��");
	}
}

//
// ������setMetaInfo(YCMagicMeta* magicMeta)
//
// Ŀ�ģ����Ӽ���Ԫ���ݶ���
//
void YCMagicMetaManager::setMetaInfo(YCMagicMeta* magicMeta)
{
	if (magicMeta == NULL)
	{
		throw YCException(2002, "YCMagicMetaManager::setMetaInfoԪ����Ϊ��");
	}

	unsigned int magicId = magicMeta->getMagicId();
	std::map<unsigned int, YCMagicMeta*>::iterator at = myMagicMetas.find(magicId);
	if (at != myMagicMetas.end())
	{
		throw YCException(2002, "YCMagicMetaManager::setMetaInfo��magicId�ظ�");
	}

	myMagicMetas[magicId] = magicMeta;
}

//
// ������getMetaInfo(unsigned int magicId)
//
// Ŀ�ģ���ȡ����Ԫ���ݶ���
//
YCMagicMeta* YCMagicMetaManager::getMetaInfo(unsigned int magicId)
{
	std::map<unsigned int, YCMagicMeta*>::iterator at = myMagicMetas.find(magicId);
	if (at == myMagicMetas.end())
	{
		throw YCException(2002, "YCMagicMetaManager::getMetaInfo��magicIdΪ��");
	}

	return at->second;
}