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
		LOG_ERROR("YCMagicMetaManager::~YCMagicMetaManager释放技能元数据失败");
	}
}

//
// 函数：setMetaInfo(YCMagicMeta* magicMeta)
//
// 目的：增加技能元数据对象
//
void YCMagicMetaManager::setMetaInfo(YCMagicMeta* magicMeta)
{
	if (magicMeta == NULL)
	{
		throw YCException(2002, "YCMagicMetaManager::setMetaInfo元数据为空");
	}

	unsigned int magicId = magicMeta->getMagicId();
	std::map<unsigned int, YCMagicMeta*>::iterator at = myMagicMetas.find(magicId);
	if (at != myMagicMetas.end())
	{
		throw YCException(2002, "YCMagicMetaManager::setMetaInfo中magicId重复");
	}

	myMagicMetas[magicId] = magicMeta;
}

//
// 函数：getMetaInfo(unsigned int magicId)
//
// 目的：获取技能元数据对象
//
YCMagicMeta* YCMagicMetaManager::getMetaInfo(unsigned int magicId)
{
	std::map<unsigned int, YCMagicMeta*>::iterator at = myMagicMetas.find(magicId);
	if (at == myMagicMetas.end())
	{
		throw YCException(2002, "YCMagicMetaManager::getMetaInfo中magicId为空");
	}

	return at->second;
}