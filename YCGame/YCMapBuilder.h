#ifndef _INCLUDE_YCMAPBUILDER_H_
#define _INCLUDE_YCMAPBUILDER_H_

class YCTiledMap;

//
// YCMapBuilder: ��ͼ������
//
// ˵������ͼ������
//       ���е�ͼ�������YCConfig��TiledMap���õ�Ŀ¼����
//       �����ͼID��1��ʼ������
//       Ŀ¼�µ�0���ļ��д�Ź���ͼƬ�����赲��ͼ���
//       ����ͼƬ���������ִ�ţ�����YCTextureManagerͳһ��������
//
struct YCMapBuilder
{
	//
	// ������build(unsigned int mapId, const char* filename)
	//
	// Ŀ�ģ������ͼ����
	//
	static YCTiledMap* build(unsigned int mapId, const char* filename);

};

#endif

