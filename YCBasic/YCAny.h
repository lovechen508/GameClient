#ifndef _INCLUDE_YCANY_H_
#define _INCLUDE_YCANY_H_

#include <iostream>  
#include <list> 

#include "YCDef.h"
#include "YCAssert.h"

//�Զ����any��  
class YCAny  
{  
public:  
      
    //�����������ݵĽӿ���  
    class placeholder  
    {  
    public:       
        virtual ~placeholder()  
        {  
        }  
    public:   
 
        virtual const std::type_info & type() const = 0;  
        virtual placeholder * clone() const = 0;      
    };  
 
    //��������ͻ�ȡ���ݵ��ࡣ  
    template<typename ValueType>  
    class holder : public placeholder  
    {  
    public:           
        holder(const ValueType & value): held(value)  
        {  
        }  
 
    public:   
 
        virtual const std::type_info & type() const 
        {  
            return typeid(ValueType);  
        }  
 
        virtual placeholder * clone() const 
        {  
            return new holder(held);//ʹ����ԭ��ģʽ  
        }  
 
    public:   
 
        //���������ݣ��ͱ���������  
        ValueType held;  
    };  
 
public:  
 
    YCAny(): content(NULL)     
    {         
    }  
 
    //ģ�幹�캯���������������������ͣ����������ݱ�����content��  
    template<typename ValueType>  
    YCAny(const ValueType & value): content(new holder<ValueType>(value))  
    {  
    }    
 
    //�������캯��  
    YCAny(const YCAny& other)  
        : content(other.content ? other.content->clone() : 0)  
    {  
    }  

	//��ֵ����������
	YCAny& operator=(const YCAny& other)
	{
		content = other.content 
			    ? other.content->clone() 
				: 0;
		return *this;
	}
 
    //����������ɾ���������ݵ�content����  
    ~YCAny()  
    {  
       SAFE_DELETE(content);  
    }  
 
    //��ѯ��ʵ���ݵ����͡�  
    const std::type_info & type() const 
    {  
        return content ? content->type() : typeid(void);  
    }  

private:  
    //һ��placeholde����ָ�룬ָ��������folder��һ��ʵ��  
    // ��content( new holder<ValueType>(value) )���  
    placeholder* content;  
 
    template<typename ValueType> friend ValueType any_cast(const YCAny& operand);  
};

//��ȡcontent->helder���ݵķ�����������ȡ����������  
template<typename ValueType>  
ValueType any_cast(const YCAny& operand)  
{  
	SMART_ASSERT( operand.type() == typeid(ValueType) );  
	return static_cast<YCAny::holder<ValueType> *>(operand.content)->held;  
}

#endif