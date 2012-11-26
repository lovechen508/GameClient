#ifndef _INCLUDE_YCGAME_H_
#define _INCLUDE_YCGAME_H_

#include "YCIGameStage.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCIFsm.h"

class YCLua;
class YCInput;
class YCConfig;
class YCNetwork;
class YCGraphic;
class YCUIManager;
class YCModuleManager;

/*
 * YCGame : ��Ϸ���ƺ�����
 *  
 * ע�ͣ�
 *     �������� YCNetwork
 *     ��Ϸ���� YCData
 *
 *                          Launcher                            ||                            GameWorld
 *																||
 *                     --------------------                     ||
 *                    |                    |                    ||
 *                    |                    |                    ||
 *                    |                    |                    ||                            Failed                            CreateRole
 *                    |   GameNew, Forum   |                    ||                        --------------                      --------------
 *                    |    Game Update     |                    ||                       |              |                    |              |
 *                    |                    |                    ||                       V              |                    V              |
 *                    |                    |                    ||                 ---------------      |		      ---------------       |			  ---------------  
 *                    |                    |      IP��Port      ||                |			      |-----		     |			      |-----			 |			      |
 *                    |    Game Section    | -------------------||--------------> |     Login     | ---------------> |   SelectRole   | ---------------> |    GameWorld   |
 *	     Launch       |    ------------    |                    ||                |				  |   successful     |				  |   successful     |				  |
 *	----------------->|                    |                    ||                 ---------------				      ---------------					  ---------------
 *		              |                    |                    || 						  |									|                                    |
 *		              |                    |                    ||						  |									|                                    |
 *		              |    Game Portal     |                    ||						  |									|               ReselectRole		 |
 *		              |    (IP, Port)      |                    ||						  |									<------------------------------------|
 *		              |                    |                    ||						  |																		 |
 *		              |                    |                    ||						  |                          Logout										 |
 *		              |                    |                    ||						   <---------------------------------------------------------------------|
 *		              |      AntiVirs      |                    ||																								 |
 *		              |                    |                    ||						               Back to choose Game Section                               |
 *		              |                    |<-------------------||-----------------------------------------------------------------------------------------------|
 *		              |                    |                    ||
 *                     --------------------                     ||
 *     ...
 */
class YCAPI YCGame
	: public YCIFsm
{
public:

	YCGame();

	~YCGame();

	//
	// �����飺����ȫ��״̬
	//
	YCLua* getLua() const;
	YCNetwork* getNetwork() const;
	YCGraphic* getGraphic() const;
	YCUIManager* getUIManager() const;

	//
	// ������initialize(HWND hWnd, YCInput& input, YCConfig& config)
	//
	// Ŀ�ģ���ʼ����Ϸ������
	//
	bool initialize(HWND hWnd, YCInput& input, YCConfig& config);

	//
	// ������bindGraphic(YCGraphic* graphic)
	//
	// Ŀ�ģ���ͼ������
	//
	void bindGraphic(YCGraphic* graphic);

	//
	// ������launchStage(YCConfig& config)
	//
	// Ŀ�ģ�������Ϸ����
	//
	void launchStage(YCConfig& config);

	//
	// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ���Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷ���Ϸ������
	//
	void finalize();

private:

	//
	// ������initializeController()
	//
	// Ŀ�ģ���YCUIManagerע��YCIController
	//
	void initializeController();

private:

	HWND myWnd;

	//
	// Lua�������
	//
	YCLua *myLua;

	//
	// ����ӿڹ�������������������
	//
	YCNetwork *myNetwork;

	//
	// ͼ�νӿڣ���������������
	//
	YCGraphic *myGraphic;

	//
	// �������������������������
	//
	YCUIManager *myUIManager;
	
	//
	// ģ���������������������
	//
	YCModuleManager* myModuleManager;
};

#endif