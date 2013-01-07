#ifndef _INCLUDE_YCIMECTRL_H_
#define _INCLUDE_YCIMECTRL_H_

//
// YCImeCtrl : ���뷨������
//            
//   ˵����
//         Ҫ��������뷨������Ҫ�����������������ϵͳ�Ѱ�װ�����뷨��Ϣ��
//         ��Windowsƽ̨�£�ÿ����װ�����뷨����ע�����ע���������Ϣ��
//         �ڡ�HKEY_CURRENT_USER\keyboard layout\preload�����¾Ϳ����ҵ���Щ��Ϣ����������1Ϊ���ĵ���������Ϊֵ������ȡ��Ϊ���ֺţ���
//         ��ֵ��������һ���ɰ˸�������ɵ��ַ�������ȡ��Ϊ���ţ���"e0040804"��,������4λ���豸����(device identifier)����4λ�����Դ���(language identifier)��
//         �������棺��e004ָ����ABC,��0804ָ��½���ġ�
//         ��MSDN�ж����д���������ϸ��˵���������Ȥ�����MSDN������ݡ�
//         ����Ҫ˵��һ�������Windows98�汾�����뷨ע����Ϣ������˵�����в�ͬ�����ǽ��Ѱ�װ�����뷨�����ֺ���Ϊ��\Preload������Ӽ���
//         ��Windows2000�����ֺ���Ϊ��\Preload���µ�ֵ��
//       ��
//         ͨ����ȡע����е����뷨��Ϣ�������г������Ѱ�װ�����뷨�����õ������뷨��Ϣֻ��һЩ�����Ѷ������ִ�����ν���Щ���ִ�������׶�������˵���أ�
//         ͬ������HKEY_LOCAL_MACHINE:"System\CurrentControlSet\Control\Keyboard Layouts\"����ע������Щ��Ϣ��
//         �����Ӽ���Ϊ���뷨����(keyboard layout)������Ϊ�����뷨��ime�ļ������Ƶ���Ϣ��
//
class YCAPI YCImeCtrl
{
public:

    //
    // ���뷨��Ϣ�ṹ
    //
    struct IL{
        char ilID[15];      //���뷨���� HKEY_CURRENT_USER\keyboard layout\preload
        char szName[100];   //���뷨˵�� HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Keyboard Layouts
        IL* pNext;
    };

public:

    YCImeCtrl(void);

    ~YCImeCtrl(void);

    //
    // ��ʼ������ע����ж�ȡ��Ϣ
    //
    bool initialize();

    //
    //  ȡ�����뷨�б�
    //
    const IL* getHeader();

    //
    // ѡ�����뷨
    //
    bool selectIme(const IL*);

    //
    // �ͷ��ѷ�����Դ
    //
    void finalize();


private:

    IL* myILHead;

};

#endif


