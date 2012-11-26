#ifndef _INCLUDE_YCFLOWLAYOUT_H_
#define _INCLUDE_YCFLOWLAYOUT_H_

#include "YCILayout.h"

class YCIComponent;

/*
 * YCFlowLayout : 窗口缺省布局类
 *
 * 注释：从上而下布局所有元件，
 *       是YCIContainer的缺省布局类
 */

class YCAPI YCFlowLayout : public YCILayout
{
public:

	YCFlowLayout();

	virtual ~YCFlowLayout();

	//
	// 函数：addComponent(YCIComponent* component, int position)
	//
	// 目的：添加元件到某特定位置
	//
	virtual void addComponent(YCIComponent* component, int position);

	//
	// 函数：removeComponent(YCIComponent* component)
	//
	// 目的：添加元件到某特定位置
	//
	virtual bool removeComponent(YCIComponent* component);
};
#endif