#ifndef _INCLUDE_YCIGAMESTAGE_H_
#define _INCLUDE_YCIGAMESTAGE_H_

#include "YCBasic\YCIFsmState.h"

class YCLua;
class YCGame;
class YCConfig;
class YCNetwork;
class YCGraphic;
class YCUIManager;

/*
 * YCIGameStage : ��Ϸ����ת���ӿ���
 *
 * ˵����
 *       ����������������¼��ѡ��ɫ����Ϸ
 */
class YCAPI YCIGameStage 
	: public YCIFsmState
{
public:

	enum {
		LOGIN_STAGE	= 1,
		SELECTROLE_STAGE = 2,
		GAME_STAGE = 3
	};

public:
	
	YCIGameStage(YCGame* game, unsigned int stage);
	
	virtual ~YCIGameStage(void);

	//
	// ������stageNo() const
	//
	// Ŀ�ģ�ȡ��״̬���
	//
	unsigned int stageNo() const;

	//
	// ������nextState(unsigned int)
	//
	// Ŀ�ģ��Ƿ���һ״̬
	//
	virtual bool nextState(unsigned int);

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ����󴴽�����
	//
	virtual bool initialize(YCConfig& config) = 0;

	//
	// ������with(unsigned int id, T*)
	//
	// Ŀ�ģ������ڲ��������ݣ�YCIFsmTransfer����
	//      ��ʵ�֣�����ʵ��
	//
	virtual YCIFsmState* with(unsigned int id, void*);

	//
	// ����: finalize()
	//
	// Ŀ�ģ�������������
	//
	virtual void finalize() = 0;

protected:

	//
	// myStage : stage���
	//
	const unsigned int myStage;

	//
	// Lua�������
	//
	YCLua *myLua;

	//
	// ����ӿڹ�������������������
	//
	YCNetwork *myNetwork;

	//
	// ͼ�νӿڣ���������������
	//
	YCGraphic *myGraphic;

	//
	// �������������������������
	//
	YCUIManager *myUIManager;


};

#endif