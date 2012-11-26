#ifndef _INCLUDE_YCMAINPLAYER_H_
#define _INCLUDE_YCMAINPLAYER_H_

#include "YCPlayer.h"

class YCItem;
class YCIPackage;
class YCMagicTree;

/*
 * YCMainPlayer : �����
 *
 * ˵����
 *
 *
 */
class YCMainPlayer :
	public YCPlayer
{
public:
	
	YCMainPlayer(void);

	virtual ~YCMainPlayer(void);

	//
	// ������update(unsigned int)
	//
	// Ŀ�ģ������ڲ�״̬
	//
	void update(unsigned int event);

	//
	// ��������ǰ����ֵ����
	//
	// Ŀ�ģ����ã���ȡ��ǰ����ֵ
	//
	void setExp(unsigned long long exp);
	unsigned long long getExp() const;

	//
	// ��������������ֵ����
	//
	// Ŀ�ģ����ã���ȡ��������ֵ
	//
	void setLevelupExp(unsigned long long exp);
	unsigned long long getLevelupExp() const;

	//
	// ��������ǰ�������
	//
	// Ŀ�ģ����ã���ȡ�������
	//
	void setLLPD(unsigned short llpd);
	unsigned short getLLPD() const;

	//
	// ��������ǰ�������
	//
	// Ŀ�ģ����ã���ȡ�������
	//
	void setULPD(unsigned short ulpd);
	unsigned short getULPD() const;

	//
	// ��������ǰ�﹥����
	//
	// Ŀ�ģ����ã���ȡ�﹥����
	//
	void setLLPA(unsigned short llpa);
	unsigned short getLLPA() const;

	//
	// ��������ǰ�﹥����
	//
	// Ŀ�ģ����ã���ȡ�﹥����
	//
	void setULPA(unsigned short ulpa);
	unsigned short getULPA() const;

	//
	// ��������ǰħ������
	//
	// Ŀ�ģ����ã���ȡħ������
	//
	void setLLMD(unsigned short llmd);
	unsigned short getLLMD() const;

	//
	// ��������ǰħ������
	//
	// Ŀ�ģ����ã���ȡħ������
	//
	void setULMD(unsigned short ulmd);
	unsigned short getULMD() const;

	//
	// ��������ǰħ������
	//
	// Ŀ�ģ����ã���ȡħ������
	//
	void setLLMA(unsigned short llma);
	unsigned short getLLMA() const;

	//
	// ��������ǰħ������
	//
	// Ŀ�ģ����ã���ȡħ������
	//
	void setULMA(unsigned short ulma);
	unsigned short getULMA() const;

	//
	// ������MP����
	//
	// Ŀ�ģ����õ�ǰMP
	//
	void setMP(unsigned int mp);
	unsigned int getMP() const;

	//
	// ������MaxMP����
	//
	// Ŀ�ģ��������MP
	//
	void setMaxMP(unsigned int maxMP);
	unsigned int getMaxMP() const;

	//
	// �������������ֿ⺯����
	//
	// Ŀ�ģ���������
	//
	void addItem(E_PACKAGE_NO package, YCItem* item, unsigned int index);
	void removeItem(E_PACKAGE_NO package, unsigned long long id);
	YCItem* getItem(E_PACKAGE_NO package, unsigned int index);

	//
	// ���������ܺ�����
	//
	// Ŀ�ģ����ܲ���
	//
	//void addMagic(YCMagic*);
	//YCMagic* getMagic()


private:

	//
	// ��ǰ,�������辭��
	//
	unsigned long long myCurrentExp;
	unsigned long long myLevelupExp;

	//
	// ��ǰ,���MP
	//
	unsigned int myMaxMP;
	unsigned int myCurrentMP;

	//
	// ������� Physical defense
	//
	//  LL : lower limit
	//  UL : upper limit
	//
	unsigned short myLLPD;
	unsigned short myULPD;

	//
	// ������ Physical attack
	//
	unsigned short myLLPA;
	unsigned short myULPA;

	//
	// ħ��
	//
	unsigned short myLLMD;
	unsigned short myULMD;

	//
	// ħ��
	//
	unsigned short myLLMA;
	unsigned short myULMA;

	//
	// ����
	//
	YCIPackage * myPackage;

	//
	// ������
	//
	YCMagicTree * myMagics;
};

#endif

