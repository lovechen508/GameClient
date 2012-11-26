#ifndef _INCLUDE_YCIFSMTRANSFER_H_
#define _INCLUDE_YCIFSMTRANSFER_H_

class YCIFsmState;

/*
 * YCIFsmTransfer : 携带状态转换的必备信息
 */
class YCIFsmTransfer
{
public:

	YCIFsmTransfer() {}

	virtual ~YCIFsmTransfer() {}

	//
	// 函数：apply(YCIFsmState *state)
	//
	// 目的：将必要信息应用到状态中
	//
	virtual void apply(YCIFsmState *state) = 0;

};

#endif