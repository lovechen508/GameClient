#ifndef _INCLUDE_YCIFSMTRANSFER_H_
#define _INCLUDE_YCIFSMTRANSFER_H_

class YCIFsmState;

/*
 * YCIFsmTransfer : Я��״̬ת���ıر���Ϣ
 */
class YCIFsmTransfer
{
public:

	YCIFsmTransfer() {}

	virtual ~YCIFsmTransfer() {}

	//
	// ������apply(YCIFsmState *state)
	//
	// Ŀ�ģ�����Ҫ��ϢӦ�õ�״̬��
	//
	virtual void apply(YCIFsmState *state) = 0;

};

#endif