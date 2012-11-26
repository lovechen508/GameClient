#ifndef _INCLUDE_YCCSSCONTEXT_H_
#define _INCLUDE_YCCSSCONTEXT_H_

class YCDList;
class YCIUITag;
class YCCSSNode;

/*
 * YCCSSContext : ���YCIContainer��CSS������
 */
class YCCSSContext
{

public:

	YCCSSContext(void);

	~YCCSSContext(void);

	//
	// ������addNode(YCCSSNode* node)
	//
	// Ŀ�ģ����YCCSSNode�ڵ�
	//
	void addNode(YCCSSNode* node);

	//
	// ������clearWithoutDelete()
	//
	// Ŀ�ģ�YCIContainer�ų�����������
	//      YCUITag�����ӽڵ㲻������������
	//
	void clearWithoutDelete();

	//
	// ������nodes() const
	//
	// Ŀ�ģ���¶����YCCSSNode�Թ�YCIUITag����
	//
	const YCDList* nodes() const;

	//
	// ������filter(const YCIUITag* parent, const YCIUITag* child)
	//
	// Ŀ�ģ�narrow down CSS������
	//
	YCCSSContext* filter(const YCIUITag* parent, const YCIUITag* child) const;

private:

	YCDList *myDList;
};

#endif