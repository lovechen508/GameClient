#ifndef _INCLUDE_YCFILETEXTURELOADER_H_
#define _INCLUDE_YCFILETEXTURELOADER_H_

class YCConfig;
class YCITextureFile;

/*
 * YCFileTextureLoader : 本地文件纹理加载器
 */
class YCFileTextureLoader 
{
public:

	YCFileTextureLoader();

	virtual ~YCFileTextureLoader();
	
	//
	// 函数：configure()
	//
	// 目的：加载纹理相关配置
	//
	void configure(YCConfig& config);

	//
	// 函数：load(const char* filename)
	//
	// 目的：加载纹理接口实现函数
	//       
	// 参数：filename为完整路径名
	//
	virtual YCITextureFile* load(const char* filename);

private:

	//
	// 函数
	//
	//
	//

	//LPDIRECT3DDEVICE9指针
	void * myLPDevice;
};

#endif