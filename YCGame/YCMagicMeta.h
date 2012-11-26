#ifndef _INCLUDE_YCMAGICMETA_H_
#define _INCLUDE_YCMAGICMETA_H_

#include "YCObjectDef.h"

#include "YCBasic\YCIDumper.h"

class YCMagicMetaImpl;

/*
 * YCMagicMeta : 技能元数据信息
 */
class YCMagicMeta
	: public YCIDumper
{
public:
	YCMagicMeta(unsigned int magicId,
		        const char* name,
				E_MAGIC_TYPE type,
				E_EQUIP_TYPE equipNeed,
				unsigned int target,
				unsigned char levelNeed,
				const char* preMagicId,     /* 1,3,4,...*/
				unsigned int prePower,
				unsigned int preIntellect,
				unsigned int attack,
				unsigned int manaAttack,
				unsigned int remoteAttack,
				unsigned int defence,
				unsigned int manaDefence,
				unsigned int HPaddOn,
				unsigned int MPaddOn,
				unsigned int range,
				unsigned int probability,
				const char* buff,          /* 1,3,5,... */
				unsigned int durationInMilliSecond,
				unsigned int intervalInMilliSecond,
				unsigned int exp,
				unsigned int icon,
				unsigned int audio,
				const char* description);

	~YCMagicMeta(void);

	//
	// 函数：dump()
	//
	// 目的：dump类内部状态为Html片段
	//
	virtual const char* dump();

	//
	// 函数：属性获取基本函数组
	//
	unsigned int  getMagicId() const;
	const char*   getName() const;
	E_MAGIC_TYPE  getType() const;
	unsigned int  getTarget() const;
	E_EQUIP_TYPE  getEquipNeed() const;
	unsigned char getLevelNeed() const;
//	const std::vector<unsigned int>& getPreMagicId() const;     /* 1,3,4 ...*/
	unsigned int  getPrePower() const;
	unsigned int  getPreIntellect() const;
	unsigned int  getAttack() const;
	unsigned int  getManaAttack() const;
	unsigned int  getRemoteAttack() const;
	unsigned int  getDefence() const;
	unsigned int  getManaDefence() const;
	unsigned int  getHPaddOn() const;
	unsigned int  getMPaddOn() const;
	unsigned int  getRange() const;
	unsigned int  getProbability() const;
//	const std::vector<unsigned int>& getBuff() const;          /* 1,3,5 ... */
	unsigned int  getDurationInMilliSecond() const;
	unsigned int  getIntervalInMilliSecond() const;
	unsigned int  getExp() const;
	unsigned int  getIcon() const;
	unsigned int  getAudio() const;
	const char*   getDescription() const;

private:

	YCMagicMetaImpl * myImpl;
	
};

#endif

