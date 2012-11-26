#ifndef _INCLUDE_YCTILEDMAPTILE_H_
#define _INCLUDE_YCTILEDMAPTILE_H_

#include "YCGraphic\YCIMapTile.h"

/*
 * YCTiledMapTile: ��ͼ�����
 *
 * ע�ͣ�
 *
 */
class YCTiledMapTile :
	public YCIMapTile
{
public:

	YCTiledMapTile(void);

	virtual ~YCTiledMapTile(void);
	
	//
	// ������setFlag(unsigned int flag) const
	//
	// Ŀ�ģ����ñ�־λ
	//
	virtual void setFlag(unsigned int flag) const;

	//
	// ������isEnabled(unsigned int flag)
	//
	// Ŀ�ģ����Ա�־λ�Ƿ�����
	//
	virtual bool isEnabled(unsigned int flag) const;

	
	//
	// ������getIntProperty(unsigned int property) const
	//
	// Ŀ�ģ�ȡ��Integer��������ֵ
	//
	virtual int getIntProperty(unsigned int property) const;
	
	//
	// ������getFloatProperty(unsigned int property) const
	//
	// Ŀ�ģ�ȡ��Float��������ֵ
	//
	virtual float getFloatProperty(unsigned int property) const;
		
	//
	// ������getStringProperty(unsigned int property) const
	//
	// Ŀ�ģ�ȡ���ַ�����������ֵ
	//
	virtual const char* getStringProperty(unsigned int property) const;

private:


};

#endif



