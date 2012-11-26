#include "YCMagicMetaBuilder.h"

#include "YCMagicMeta.h"
#include "YCMagicMetaManager.h"

#include "YCConfig\YCXmlReader.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

//////////////////////////////////////
// YCMMBHelper �ڲ���
//////////////////////////////////////
class YCMMBHelper
{
public:

	YCMMBHelper(YCMagicMetaManager* manager)
		: myManager(manager)
	{
	}

	~YCMMBHelper() {}

	//
	// ������buildBasicMagic()
	//
	// Ŀ�ģ�������������Ԫ����
	//
	bool buildBasicMagic(YCXmlReader& magic)
	{
		int id, type, equip, target, level, prePower, preIntellect, attack, manaAttack, remoteAttack, defence, manaDefence, hpAddon, mpAddon, range, probability, duration, interval, exp, icon, audio; 
		char* name = NULL;
		char* preMagic = NULL;
		char* buff = NULL;
		char* description = NULL;

		magic.attr("id", id)
			 .attr("type", type)
			 .attr("equip", equip)
			 .attr("target", target)
			 .attr("level", level)
			 .attr("prePower", prePower)
			 .attr("preIntellect", preIntellect)
			 .attr("attack", attack)
			 .attr("manaAttack", manaAttack)
			 .attr("remoteAttack", remoteAttack)
			 .attr("defence", defence)
			 .attr("manaDefence", manaDefence)
			 .attr("hpAddon", hpAddon)
			 .attr("mpAddon", mpAddon)
			 .attr("range", range)
			 .attr("probability", probability)
			 .attr("duration", duration)
			 .attr("interval",interval)
			 .attr("exp", exp)
			 .attr("icon", icon)
			 .attr("audio", audio)
			 .attr("name", &name)
			 .attr("preMagic", &preMagic)
			 .attr("buff", &buff)
			 .attr("description", &description);

		myManager->setMetaInfo(new YCMagicMeta(id, name, (E_MAGIC_TYPE)type, (E_EQUIP_TYPE)equip, target, 
			level, preMagic, prePower, preIntellect, attack, manaAttack, remoteAttack, defence, manaDefence,
			hpAddon, mpAddon, range, probability, buff, duration, interval, exp, icon, audio, description));

		SAFE_DELETE(name);
		SAFE_DELETE(preMagic);
		SAFE_DELETE(buff);
		SAFE_DELETE(description);

		return true;
	}

private:

	YCMagicMetaManager* myManager;
};

//
// ������build(const char* filename, YCMagicMetaManager* manager)
//
// Ŀ�ģ����������ļ���������Ԫ����
//
bool YCMagicMetaBuilder::build(const char* filename, YCMagicMetaManager* manager)
{
	if (filename == NULL)
	{
		throw YCException(2002, "YCMagicMetaBuilder::build�ļ���Ϊ��");
	}

	if (manager == NULL)
	{
		throw YCException(2002, "YCMagicMetaBuilder::build������Ϊ��");
	}

	//TODO ...
	YCXmlReader reader(filename);

	reader.enter("basic")
		    .foreach("magic", YCXmlReader::YCForEachWrapper<YCMMBHelper>(&YCMMBHelper(manager), &YCMMBHelper::buildBasicMagic))
		  .leave();

	return true;
}
