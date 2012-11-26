#ifndef _INCLUDE_YCIMODEL_H_
#define _INCLUDE_YCIMODEL_H_

class YCDList;
class YCIComponent;

/*
 * Model �� 
 *   ����CPP�ķ������ʵ��Model��View�İ�
 *
 *                                                     ---------------------
 *                                                    |      YCIModel       |
 *                                                     ---------------------
 *                                                               |
 *                                                     ---------------------
 *                                                    |      YCISprite      |
 *                                                     ---------------------
 *                                                               |
 *                    ---------------------------------------------------------------------------------------
 *                   |                          |                            |                               |
 *            ---------------             ----------------            -------------                  ------------------
 *           |    YCItem     |           | YCIMessageUnit |          |   YCMagic   |                |    YCMapObject   |
 *            ---------------             ----------------            -------------                  ------------------
 *                                                                                                           |
 *                                                                     ---------------------------------------------------------------------------
 *                                                                    |                                                                           |
 *                                                             ---------------                                                             ---------------
 *                                                            |  YCCreature   |                                                           | YCMagicEffect |
 *                                                             ---------------                                                             ---------------
 *                                                                    |
 *                                 --------------------------------------------------------------------------
 *                                |                                   |                                      |
 *                         --------------                        -----------                          --------------
 *                        |   YCMonster  |                      |   YCNpc   |                        |   YCPlayer   |
 *                         --------------                        -----------                          --------------
 *                                                                                                           |
 *                                                                                                    --------------
 *                                                                                                   | YCMainPlayer |
 *                                                                                                    --------------
 */

class YCAPI YCIModel  
{
public:

	enum E_MODEL_STATE{MODEL_UPDATED, MODEL_DELETE} E_MODEL_STATE;

public:

	YCIModel(void);

	virtual ~YCIModel(void);

	//
	// ������initialize()
	//
	// Ŀ�ģ�����ģ�ͳ�ʼ��
	//
	//virtual bool initialize() = 0;

	//
	// ������subscribe(YCIComponent*)
	//
	// Ŀ�ģ��󶨶�����ģ�͵ļ���
	//       Ŀǰ�������ʵ����YCIComponent�䵱��View��Controller���ߵĽ�ɫ
	//
	void subscribe(YCIComponent* ui);

	//
	// ������unsubscribe(YCIComponent*)
	//
	// Ŀ�ģ�ȡ��������ģ�͵ļ���
	//       Ŀǰ�������ʵ����YCIComponent�䵱��View��Controller���ߵĽ�ɫ
	//
	void unsubscribe(YCIComponent* ui);

	//
	// ������finialize()
	//
	// Ŀ�ģ��ͷ�����ģ���е���Դ
	//
	//virtual void finialize() = 0;

private:

	YCDList * mySubscribers;
	
};

#endif
