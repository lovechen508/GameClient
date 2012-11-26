#ifndef _INCLUDE_YCFILETEXTURELOADER_H_
#define _INCLUDE_YCFILETEXTURELOADER_H_

class YCConfig;
class YCITextureFile;

/*
 * YCFileTextureLoader : �����ļ����������
 */
class YCFileTextureLoader 
{
public:

	YCFileTextureLoader();

	virtual ~YCFileTextureLoader();
	
	//
	// ������configure()
	//
	// Ŀ�ģ����������������
	//
	void configure(YCConfig& config);

	//
	// ������load(const char* filename)
	//
	// Ŀ�ģ���������ӿ�ʵ�ֺ���
	//       
	// ������filenameΪ����·����
	//
	virtual YCITextureFile* load(const char* filename);

private:

	//
	// ����
	//
	//
	//

	//LPDIRECT3DDEVICE9ָ��
	void * myLPDevice;
};

#endif