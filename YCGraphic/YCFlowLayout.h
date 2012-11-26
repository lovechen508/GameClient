#ifndef _INCLUDE_YCFLOWLAYOUT_H_
#define _INCLUDE_YCFLOWLAYOUT_H_

#include "YCILayout.h"

class YCIComponent;

/*
 * YCFlowLayout : ����ȱʡ������
 *
 * ע�ͣ����϶��²�������Ԫ����
 *       ��YCIContainer��ȱʡ������
 */

class YCAPI YCFlowLayout : public YCILayout
{
public:

	YCFlowLayout();

	virtual ~YCFlowLayout();

	//
	// ������addComponent(YCIComponent* component, int position)
	//
	// Ŀ�ģ����Ԫ����ĳ�ض�λ��
	//
	virtual void addComponent(YCIComponent* component, int position);

	//
	// ������removeComponent(YCIComponent* component)
	//
	// Ŀ�ģ����Ԫ����ĳ�ض�λ��
	//
	virtual bool removeComponent(YCIComponent* component);
};
#endif