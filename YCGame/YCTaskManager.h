#ifndef _INCLUDE_YCTASKMANAGER_H_
#define _INCLUDE_YCTASKMANAGER_H_

class YCConfig;

struct NET_TASK_MSG_NTF_T;

#include "YCGraphic/YCIModel.h"

/*
 * YCTaskManager�����������
 *
 */
class YCAPI YCTaskManager : public YCIModel
{

public:

	YCTaskManager(void);

	virtual ~YCTaskManager(void);

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ���ʼ�����������
	//
	bool initialize(YCConfig& config);

	//
	// ������recv(const NET_TASK_MSG_NTF_T* msg)
	//
	// Ŀ�ģ���Ϣ����
	//
	void recv(const NET_TASK_MSG_NTF_T* msg);

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷ���Դ
	//
	void finalize();

private:

    //
    //
    //

};

#endif

