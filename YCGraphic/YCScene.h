#ifndef _INCLUDE_YCSCENE_H_
#define _INCLUDE_YCSCENE_H_

#include <Windows.h>

#include "YCSceneDef.h"

#include "YCInput\YCIInputListener.h"

class YCDList;
class YCAudio;
struct YCSpriteChain;
class YCISceneLayer;
class YCGraphic;

class YCAPI YCScene
	: public YCIInputListener
{
public:
	YCScene(YCGraphic *graphic);

	virtual ~YCScene(void);

	//
	// ������setPosition(unsigned int x, unsigned int y, unsigned int offsetX, unsigned int offsetY)
	//
	// Ŀ�ģ����ó�����������
	//
	void setPosition(unsigned int x,
		             unsigned int y,
					 unsigned int offsetX,
					 unsigned int offsetY);
				
	//
	// ������initialize()
	//
	// Ŀ�ģ���ʼ��������
	//
	bool initialize();

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷų�����
	//
	void finalize();

	//
	// ������onLostDevice()
	// 
	// Ŀ�ģ���ͼ��ϵͳ��ʧ�豸ʱ��YCGraphic����
	//
	void onLostDevice();

	//
	// ������onResetDevice()
	// 
	// Ŀ�ģ���ͼ��ϵͳ�����豸ʱ��YCGraphic����
	//
	void onResetDevice();

	//
	// ������addLayer(YCISceneLayer *layer)
	//
	// Ŀ�ģ�
	//
	void addLayer(YCISceneLayer *layer);

	//
	// ������removeLayer(YCISceneLayer *layer)
	//
	// Ŀ�ģ��ӳ������Ƴ�һ�㣬ע��layer�ڴ��Ѿ��ͷ�
	//      ��������true��Ӧ��ʹ��layer
	//
	bool removeLayer(YCISceneLayer* layer);

	//
	// ������getLayer(E_SCENELAYER_ZORDER zOrder)
	//
	// Ŀ�ģ�
	//
	YCISceneLayer* getLayer(E_SCENELAYER_ZORDER zOrder);

	//
	// ������render()
	//
	// Ŀ�ģ���Ⱦ�����������������㣬��ZOrder��С����˳����Ⱦ
	//
	virtual void render();

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	//
	// ���ԣ�myXPosition
	//       myYPosition
	//       myXOffset
	//       myYOffset
	//
	// Ŀ�ģ��������ĵ��ͼ������(x, y)
	//       �����ڸÿ��ڵ�ƫ����
	//
	unsigned int myXPosition;
	unsigned int myYPosition;
	unsigned int myXOffset;
	unsigned int myYOffset;

	/*
	 * Zordered ������
	 *	�����û�ԽԶԽ����
	 *	��Ⱦ��ʱ��Ӻ���ǰ��Ⱦ
	 *	���ڵ�����������ں���50% Alpha����Ⱦһ��
	 *	��������˳��Ϊ��
	 *    UI -> ����50% -> �ڵ� -> ���� -> С�ر� -> ��ر� -> ��ر�(˫��ر�
	 */
	YCDList* mySceneLayers;
	
	YCAudio * myAudio;
	
	YCSpriteChain *mySpriteHead;

	YCGraphic * myGraphic;
};

#endif
