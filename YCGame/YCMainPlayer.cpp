#include "YCMainPlayer.h"

#include "YCItem.h"
#include "YCMagic.h"
#include "YCPackage.h"
#include "YCMagicTree.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCMainPlayer::YCMainPlayer(void)
	: myPackage(new YCPackage())
	, myMagics(new YCMagicTree())
{
}


YCMainPlayer::~YCMainPlayer(void)
{
	SAFE_DELETE(myPackage);
	SAFE_DELETE(myMagics);
}

//
// ������update(unsigned int)
//
// Ŀ�ģ������ڲ�״̬
//
void YCMainPlayer::update(unsigned int event)
{

}

//
// ��������ǰ����ֵ����
//
// Ŀ�ģ����ã���ȡ��ǰ����ֵ
//
void YCMainPlayer::setExp(unsigned long long exp)
{
	myCurrentExp = exp;
}

unsigned long long YCMainPlayer::getExp() const
{
	return myCurrentExp;
}

//
// ��������������ֵ����
//
// Ŀ�ģ����ã���ȡ��������ֵ
//
void YCMainPlayer::setLevelupExp(unsigned long long exp)
{
	myLevelupExp = exp;
}

unsigned long long YCMainPlayer::getLevelupExp() const
{
	return myLevelupExp;
}

//
// ��������ǰ�������
//
// Ŀ�ģ����ã���ȡ�������
//
void YCMainPlayer::setLLPD(unsigned short llpd)
{
	myLLPD = llpd;
}

unsigned short YCMainPlayer::getLLPD() const
{
	return myLLPD;
}

//
// ��������ǰ�������
//
// Ŀ�ģ����ã���ȡ�������
//
void YCMainPlayer::setULPD(unsigned short ulpd)
{
	myULPD = ulpd;
}

unsigned short YCMainPlayer::getULPD() const
{
	return myULPD;
}

//
// ��������ǰ�﹥����
//
// Ŀ�ģ����ã���ȡ�﹥����
//
void YCMainPlayer::setLLPA(unsigned short llpa)
{
	myLLPA = llpa;
}

unsigned short YCMainPlayer::getLLPA() const
{
	return myLLPA;
}

//
// ��������ǰ�﹥����
//
// Ŀ�ģ����ã���ȡ�﹥����
//
void YCMainPlayer::setULPA(unsigned short ulpa)
{
	myULPA = ulpa;
}

unsigned short YCMainPlayer::getULPA() const
{
	return myULPA;
}

//
// ��������ǰħ������
//
// Ŀ�ģ����ã���ȡħ������
//
void YCMainPlayer::setLLMD(unsigned short llmd)
{
	myLLMD = llmd;
}

unsigned short YCMainPlayer::getLLMD() const
{
	return myLLMD;
}

//
// ��������ǰħ������
//
// Ŀ�ģ����ã���ȡħ������
//
void YCMainPlayer::setULMD(unsigned short ulmd)
{
	myULMD = ulmd;
}

unsigned short YCMainPlayer::getULMD() const
{
	return myULMD;
}

//
// ��������ǰħ������
//
// Ŀ�ģ����ã���ȡħ������
//
void YCMainPlayer::setLLMA(unsigned short llma)
{
	myLLMA = llma;
}

unsigned short YCMainPlayer::getLLMA() const
{
	return myLLMA;
}

//
// ��������ǰħ������
//
// Ŀ�ģ����ã���ȡħ������
//
void YCMainPlayer::setULMA(unsigned short ulma)
{
	myULMA = ulma;
}

unsigned short YCMainPlayer::getULMA() const
{
	return myULMA;
}

//
// ������MP����
//
// Ŀ�ģ����õ�ǰMP
//
void YCMainPlayer::setMP(unsigned int mp)
{
	if (mp > myMaxMP)
	{
		throw YCException(2002, "YCPlayer::setHPѪ���������ֵ");
	}

	myCurrentMP = mp;
}

unsigned int YCMainPlayer::getMP() const
{
	return myCurrentMP;
}

//
// ������MaxMP����
//
// Ŀ�ģ��������MP
//
void YCMainPlayer::setMaxMP(unsigned int maxMP)
{
	myMaxMP = maxMP;
}

unsigned int YCMainPlayer::getMaxMP() const
{
	return myMaxMP;
}

//
// �������������ֿ⺯����
//
// Ŀ�ģ���������
//
void YCMainPlayer::addItem(E_PACKAGE_NO package, YCItem* item, unsigned int index)
{
	if (item == NULL)
	{
		throw YCException(2002, "YCMainPlayer::addItem��ƷΪ��");
	}

	//  �������
	if (package == PACKAGE_HANDY)
	{
		myPackage->setItemAt(index, item);
	}
}

void YCMainPlayer::removeItem(E_PACKAGE_NO package, unsigned long long id)
{
	if (package == PACKAGE_HANDY)
	{
		YCItem* item = myPackage->findById(id);
		myPackage->removeItemAt(item->getIndex());

		SAFE_DELETE(item);
	}
}

YCItem* YCMainPlayer::getItem(E_PACKAGE_NO package, unsigned int index)
{
	YCItem* found = NULL;
	if (package == PACKAGE_HANDY)
	{
		found = myPackage->getItemAt(index);
	}

	return found;
}