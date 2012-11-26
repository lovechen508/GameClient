#ifndef _INCLUDE_YCICLONEABLE_H_
#define _INCLUDE_YCICLONEABLE_H_

/*
 * YCICloneable : 可克隆接口类
 */
template<typename T>
class YCICloneable
{
public:

	//
	// 函数：clone()
	//
	// 目的：克隆当前对象
	//
	virtual T* clone() = 0;
};

#endif