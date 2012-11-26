#include "YCMagicMeta.h"

#include <vector>

#include "YCBasic\YCDef.h"
#include "YCBasic\YCStringHelper.h"
#include "YCBasic\YCException.h"

/////////////////////////////////////////////////
// Class YCMagicMetaImpl 内部实现类
/////////////////////////////////////////////////
class YCMagicMetaImpl
{
public:

	YCMagicMetaImpl(unsigned int magicId,
					const char* name,
					E_MAGIC_TYPE type,
					E_EQUIP_TYPE equipNeed,
					unsigned int target,
					unsigned char levelNeed,
					const char* preMagicId,
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
					const char* buff,
					unsigned int durationInMilliSecond,
					unsigned int intervalInMilliSecond,
					unsigned int exp,
					unsigned int icon,
					unsigned int audio,
					const char* description)
		: myMagicId(magicId)
		, myName(name)
		, myType(type)
		, myEquipNeed(equipNeed)
		, myTarget(target)
		, myLevelNeed(levelNeed)
		, myPrePower(prePower)
		, myPreIntellect(preIntellect)
		, myAttack(attack)
		, myManaAttack(manaAttack)
		, myRemoteAttack(remoteAttack)
		, myDefence(defence)
		, myManaDefence(manaDefence)
		, myHPaddOn(HPaddOn)
		, myMPaddOn(MPaddOn)
		, myRange(range)
		, myProbability(probability)
		, myDurationInMilliSecond(durationInMilliSecond)
		, myIntervalInMilliSecond(intervalInMilliSecond)
		, myExp(exp)
		, myIcon(icon)
		, myAudio(audio)
		, myDescription(description)
	{
		parsePreMagic(preMagicId);
		parseBuff(buff);
	}

	~YCMagicMetaImpl() {}

	unsigned int  getMagicId() const						{ return myMagicId;				  }
	const char*   getName() const							{ return myName.c_str();          }
	E_MAGIC_TYPE  getType() const							{ return myType;                  }
	unsigned int  getTarget() const							{ return myTarget;                }
	E_EQUIP_TYPE  getEquipNeed() const						{ return myEquipNeed;             }
	unsigned char getLevelNeed() const						{ return myLevelNeed;             }
	const std::vector<unsigned int>& getPreMagicId() const  { return myPreMagicId;            }     
	unsigned int  getPrePower() const						{ return myPrePower;              }
	unsigned int  getPreIntellect() const					{ return myPreIntellect;          }
	unsigned int  getAttack() const							{ return myAttack;                }
	unsigned int  getManaAttack() const						{ return myManaAttack;            }
	unsigned int  getRemoteAttack() const					{ return myRemoteAttack;		  }
	unsigned int  getDefence() const						{ return myDefence;               }
	unsigned int  getManaDefence() const					{ return myManaDefence;           }
	unsigned int  getHPaddOn() const						{ return myHPaddOn;               }
	unsigned int  getMPaddOn() const						{ return myMPaddOn;               }
	unsigned int  getRange() const							{ return myRange;                 }
	unsigned int  getProbability() const					{ return myProbability;           }
	const std::vector<unsigned int>& getBuff() const		{ return myBuff;                  }          
	unsigned int  getDurationInMilliSecond() const			{ return myDurationInMilliSecond; }
	unsigned int  getIntervalInMilliSecond() const			{ return myIntervalInMilliSecond; }
	unsigned int  getExp() const							{ return myExp;                   }
	unsigned int  getIcon() const							{ return myIcon;                  }
	unsigned int  getAudio() const							{ return myAudio;                 }
	const char*   getDescription() const					{ return myDescription.c_str();   }

private:

	//
	// 函数：parsePreMagic(const char* premagic)
	//
	// 目的：解析前置技能 逗号分隔
	//
	void parsePreMagic(const char* premagic)
	{
		if (premagic != NULL)
		{
			const char* off = premagic;
			const char* pos = premagic;
			while (pos != '\0')
			{
				if (*pos == ',')
				{
					if (pos > off)
					{
						char buf[12] = {0};
						strncpy(buf, off, pos-off);
						int aMagic = atoi(buf);
						if (aMagic > 0)
						{
							myPreMagicId.push_back(aMagic);
						}
					}

					++pos, off=pos;
				}
				else
				{
					++pos;
				}
			}
		}
	}

	//
	// 函数：parseBuff(const char* buff)
	//
	// 目的：解析Debuff
	//
	void parseBuff(const char* buff)
	{
		if (buff != NULL)
		{
			const char* off = buff;
			const char* pos = buff;
			while (pos != '\0')
			{
				if (*pos == ',')
				{
					if (pos > off)
					{
						char buf[12] = {0};
						strncpy(buf, off, pos-off);
						int aBuff = atoi(buf);
						if (aBuff > 0)
						{
							myBuff.push_back(aBuff);
						}
					}

					++pos, off=pos;
				}
				else
				{
					++pos;
				}
			}
		}
	}
private:

	unsigned int myMagicId;

	E_MAGIC_TYPE myType;

	std::string myName;

	unsigned int myTarget;

	E_EQUIP_TYPE myEquipNeed;

	unsigned char myLevelNeed;

	std::vector<unsigned int> myPreMagicId;

	unsigned int myPrePower;

	unsigned int myPreIntellect;

	unsigned int myAttack;

	unsigned int myManaAttack;

	unsigned int myRemoteAttack;

	unsigned int myDefence;

	unsigned int myManaDefence;

	unsigned int myHPaddOn;

	unsigned int myMPaddOn;

	unsigned int myRange;

	unsigned int myProbability;

	std::vector<unsigned int> myBuff;

	unsigned int myDurationInMilliSecond;

	unsigned int myIntervalInMilliSecond;

	unsigned int myExp;

	unsigned int myIcon;

	unsigned int myAudio;

	std::string myDescription;
};

YCMagicMeta::YCMagicMeta(unsigned int magicId,
						const char* name,
						E_MAGIC_TYPE type,
						E_EQUIP_TYPE equipNeed,
						unsigned int target,
						unsigned char levelNeed,
						const char* preMagicId,
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
						const char* buff,
						unsigned int durationInMilliSecond,
						unsigned int intervalInMilliSecond,
						unsigned int exp,
						unsigned int icon,
						unsigned int audio,
						const char* description)
	: myImpl(new YCMagicMetaImpl(magicId, name, type, equipNeed, target, levelNeed, preMagicId, prePower, preIntellect, attack, manaAttack,
			 remoteAttack, defence, manaDefence, HPaddOn, MPaddOn, range, probability, buff,durationInMilliSecond,
			  intervalInMilliSecond, exp, icon, audio, description))
{
}


YCMagicMeta::~YCMagicMeta(void)
{
	SAFE_DELETE(myImpl);
}

//
// 函数：dump()
//
// 目的：dump类内部状态为Html片段
//
const char* YCMagicMeta::dump()
{
	//TODO ...
	return NULL;
}

//
// 函数：getMagicId() const
//
// 目的：
//
unsigned int  YCMagicMeta::getMagicId() const						{ return myImpl->getMagicId(); }
const char*   YCMagicMeta::getName() const							{ return myImpl->getName(); }
E_MAGIC_TYPE  YCMagicMeta::getType() const							{ return myImpl->getType(); }
unsigned int  YCMagicMeta::getTarget() const						{ return myImpl->getTarget(); }
E_EQUIP_TYPE  YCMagicMeta::getEquipNeed() const						{ return myImpl->getEquipNeed(); }
unsigned char YCMagicMeta::getLevelNeed() const						{ return myImpl->getLevelNeed(); }
//const std::vector<unsigned int>& YCMagicMeta::getPreMagicId() const	{ return myImpl->getPreMagicId(); }     
unsigned int  YCMagicMeta::getPrePower() const						{ return myImpl->getPrePower(); }
unsigned int  YCMagicMeta::getPreIntellect() const					{ return myImpl->getPreIntellect(); }
unsigned int  YCMagicMeta::getAttack() const						{ return myImpl->getAttack(); }
unsigned int  YCMagicMeta::getManaAttack() const					{ return myImpl->getManaAttack(); }
unsigned int  YCMagicMeta::getRemoteAttack() const					{ return myImpl->getRemoteAttack(); }
unsigned int  YCMagicMeta::getDefence() const						{ return myImpl->getDefence(); }
unsigned int  YCMagicMeta::getManaDefence() const					{ return myImpl->getManaDefence(); }
unsigned int  YCMagicMeta::getHPaddOn() const						{ return myImpl->getHPaddOn(); }
unsigned int  YCMagicMeta::getMPaddOn() const						{ return myImpl->getMPaddOn(); }
unsigned int  YCMagicMeta::getRange() const							{ return myImpl->getRange(); }
unsigned int  YCMagicMeta::getProbability() const					{ return myImpl->getProbability(); }
//const std::vector<unsigned int>& YCMagicMeta::getBuff() const		{ return myImpl->getBuff(); }          
unsigned int  YCMagicMeta::getDurationInMilliSecond() const			{ return myImpl->getDurationInMilliSecond(); }
unsigned int  YCMagicMeta::getIntervalInMilliSecond() const			{ return myImpl->getIntervalInMilliSecond(); }
unsigned int  YCMagicMeta::getExp() const							{ return myImpl->getExp(); }
unsigned int  YCMagicMeta::getIcon() const							{ return myImpl->getIcon(); }
unsigned int  YCMagicMeta::getAudio() const							{ return myImpl->getAudio(); }
const char*   YCMagicMeta::getDescription() const					{ return myImpl->getDescription(); }