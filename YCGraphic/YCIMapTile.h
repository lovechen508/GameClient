#ifndef _INCLUDE_YCIMAPTILE_H_
#define _INCLUDE_YCIMAPTILE_H_

class YCDList;
class YCIMapObject;

/*
 * YCIMapTile : ��ͼ�������
 *
 * ע�ͣ�
 *
 */
class YCAPI YCIMapTile
{
public:

	YCIMapTile(void);

	virtual ~YCIMapTile(void);

	//
	// ������draw(unsigned int layer, unsigned int timestamp)
	//
	// Ŀ�ģ���ĳһ��
	//
	void draw(unsigned int layer, unsigned int timestamp);

	//
	// ������addObject(YCIMapObject* object)
	//
	// Ŀ�ģ��ҽӶ��󵽵�ͼ��
	//
	void addObject(YCIMapObject* object);
	
	//
	// ������setFlag(unsigned int flag) const
	//
	// Ŀ�ģ����ñ�־λ
	//
	virtual void setFlag(unsigned int flag) const = 0;

	//
	// ������isEnabled(unsigned int flag)
	//
	// Ŀ�ģ����Ա�־λ�Ƿ�����
	//
	virtual bool isEnabled(unsigned int flag) const = 0;
	
	//
	// ������getIntProperty(unsigned int property) const
	//
	// Ŀ�ģ�ȡ��Integer��������ֵ
	//
	virtual int getIntProperty(unsigned int property) const = 0;
	
	//
	// ������getFloatProperty(unsigned int property) const
	//
	// Ŀ�ģ�ȡ��Float��������ֵ
	//
	virtual float getFloatProperty(unsigned int property) const = 0;
		
	//
	// ������getStringProperty(unsigned int property) const
	//
	// Ŀ�ģ�ȡ���ַ�����������ֵ
	//
	virtual const char* getStringProperty(unsigned int property) const = 0;

private:

	//
	// ���ԣ�myObjects
	//
	// Ŀ�ģ��ҽӵ��õ�ͼ���ϵĶ��󣬲�������������
	//
	YCDList * myObjects;

};

#endif

