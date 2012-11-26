#ifndef _INCLUDE_YCMONSTER_H_
#define _INCLUDE_YCMONSTER_H_

#include "YCCreature.h"

#include "YCBasic\YCIFsm.h"

/*
 * YCMonster : ���������
 *
 * ˵����
 *          ����״̬ת��
 *
 *                     idle    :   ԭ�ط���
 *                     move    ��  �ƶ�������
 *                     attack  ��  ����
 *                     escape  ��  ����
 *
 *                     --------------                        --------------
 *                    |     Idle     | <==================> |     Move     |
 *                     --------------                        --------------
 *                           |                                     |
 *                           |                                     |
 *                           |                                     |
 *                     --------------                        --------------
 *                    |    Attack    | <==================> |    Escape    |
 *                     --------------                        --------------
 *
 */
class YCMonster 
	: public YCCreature
	, public YCIFsm
{
public:

	YCMonster(void);

	virtual ~YCMonster(void);

	//
	// ������update(unsigned int)
	//
	// Ŀ�ģ������ڲ�״̬
	//
	virtual void update(unsigned int event);

public:

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
	// ���Ѫ��
	//
	unsigned int myMaxHP;

	//
	// ��ǰѪ��
	//
	unsigned int myCurrentHP;


};

#endif

