#ifndef _INCLUDE_YCIMECTRL_H_
#define _INCLUDE_YCIMECTRL_H_

//
// YCImeCtrl : 输入法控制类
//            
//   说明：
//         要想控制输入法，首先要解决的问题是如果获得系统已安装的输入法信息。
//         在Windows平台下，每个安装的输入法都在注册表中注册了相关信息。
//         在“HKEY_CURRENT_USER\keyboard layout\preload”键下就可以找到这些信息，键下由以1为基的递增数字做为值名（暂取名为数字号），
//         其值的内容是一个由八个数字组成的字符串（暂取名为代号，如"e0040804"）,其中左4位是设备代码(device identifier)，右4位是语言代码(language identifier)。
//         例如上面：左e004指智能ABC,右0804指大陆中文。
//         在MSDN中对所有代码做了详细的说明，如感兴趣请浏览MSDN相关内容。
//         另外要说明一点的是在Windows98版本中输入法注册信息与上面说明略有不同，它是将已安装的输入法的数字号做为…\Preload下面的子键，
//         而Windows2000将数字号做为…\Preload键下的值。
//       　
//         通过读取注册表中的输入法信息，可以列出所有已安装的输入法，但得到的输入法信息只是一些让人难懂的数字串，如何将这些数字串翻译成易懂的文字说明呢？
//         同样，在HKEY_LOCAL_MACHINE:"System\CurrentControlSet\Control\Keyboard Layouts\"键下注册了这些信息，
//         它的子键名为输入法代号(keyboard layout)，内容为该输入法的ime文件，名称等信息。
//
class YCAPI YCImeCtrl
{
public:

    //
    // 输入法信息结构
    //
    struct IL{
        char ilID[15];      //输入法代号 HKEY_CURRENT_USER\keyboard layout\preload
        char szName[100];   //输入法说明 HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Keyboard Layouts
        IL* pNext;
    };

public:

    YCImeCtrl(void);

    ~YCImeCtrl(void);

    //
    // 初始化，从注册表中读取信息
    //
    bool initialize();

    //
    //  取得输入法列表
    //
    const IL* getHeader();

    //
    // 选择输入法
    //
    bool selectIme(const IL*);

    //
    // 释放已分配资源
    //
    void finalize();


private:

    IL* myILHead;

};

#endif


