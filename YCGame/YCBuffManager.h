#ifndef _INCLUDE_YCBUFFMANAGER_H_
#define _INCLUDE_YCBUFFMANAGER_H_

#include "YCGraphic\YCIModel.h"

class YDList;
class YCConfig;

//
// Buff ������
//
class YCBuffManager : public YCIModel
{
public:

	YCBuffManager(void);

	virtual ~YCBuffManager(void);

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ���ʼ����Ϣ������
	//
	bool initialize(YCConfig& config);



	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷ���Դ
	//
	void finalize();

private:

	YDList* myBuffMetas;
};

#endif

