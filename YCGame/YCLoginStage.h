#ifndef _INCLUDE_YCLOGINSTAGE_H_
#define _INCLUDE_YCLOGINSTAGE_H_

#include "YCIGameStage.h"

/*
 * YCLoginStage : ��¼�׶�
 */
class YCAPI YCLoginStage :
	public YCIGameStage
{
public:

	YCLoginStage(YCGame* game);

	virtual ~YCLoginStage(void);

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ����󴴽�����
	//
	virtual bool initialize(YCConfig& config);
	
	//
	// ������with(const char* ip)
	//
	// Ŀ�ģ���¼������IP
	//
	virtual YCIFsmState* with(unsigned int id, const char* ip);

	//
	// ������with(unsigned int id, int port)
	//
	// Ŀ�ģ���¼������Port
	//
	virtual YCIFsmState* with(unsigned int id, int port);

	//
	// ������enter()
	//
	// Ŀ�ģ�״̬����
	//
	virtual void enter();

	//
	// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ���Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// ������leave()
	//
	// Ŀ�ģ�״̬����
	//
	virtual void leave();

	//
	// ����: finalize()
	//
	// Ŀ�ģ�������������
	//
	virtual void finalize();

private:

	//
	// ������login(const char* username, const char* password)
	//
	// Ŀ�ģ���LoginServer��¼
	//
	// ע�ͣ�����������֪�����������ػ��߳�ʱ
	//       ��¼�ɹ���������mySessionId
	//
	bool login(const char* username, const char* password);

private:

	//
	// loginServer IP
	//
	char* myHost;
	
	//
	// loginServer port
	//
	int myPort;

	//
	// �ػ�ID
	//
	unsigned int mySessionId;

};

#endif

