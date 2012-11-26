#ifndef _INCLUDE_YCPLAYER_H_
#define _INCLUDE_YCPLAYER_H_

#include "YCCreature.h"
#include "YCObjectDef.h"

#include "YCBasic\YCIFsm.h"

class YCPlayer 
	: public YCCreature
	, public YCIFsm
{
public:
	
	YCPlayer(void);

	virtual ~YCPlayer(void);

	//
	// ������update(unsigned int)
	//
	// Ŀ�ģ������ڲ�״̬
	//
	virtual void update(unsigned int event);

	//
	// ��������Ϸmode����
	//
	// Ŀ�ģ����õ�ǰ��Ϸģʽ
	//
	void setMode(E_PLAY_MODE mode);
	E_PLAY_MODE getMode() const;

	//
	// ��������Ϸ��Ҽ���
	//
	// Ŀ�ģ�������Ҽ���
	//
	void setLevel(unsigned short level);
	unsigned short getLevel() const;

	//
	// ��������Ϸ����Ա�
	//
	// Ŀ�ģ���������Ա�
	//
	void setSex(E_PLAYER_SEX sex);
	E_PLAYER_SEX getSex() const;

	//
	// ������HP����
	//
	// Ŀ�ģ����õ�ǰѪ��
	//
	void setHP(unsigned int hp);
	unsigned int getHP() const;

	//
	// ������MaxHP����
	//
	// Ŀ�ģ��������Ѫ��
	//
	void setMaxHP(unsigned int maxHP);
	unsigned int getMaxHP() const;

private:

	//
	// ��Ϸģʽ����ƽ, PK...
	//
	E_PLAY_MODE		myPlayMode; 

	//
	// ���������飺
	//
	unsigned short	myLevel;  // ����
	E_PLAYER_SEX	mySex;    // �Ա�
	
	//
	// ��ǰ,���Ѫ��
	//
	unsigned int	myMaxHP;
	unsigned int	myCurrentHP;
};

#endif
