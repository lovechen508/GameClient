#ifndef _INCLUDE_YCUIMEDIATOR_H_
#define _INCLUDE_YCUIMEDIATOR_H_

//
// YCUIMediator ：中介类
//
// 说明：用于YCIModel和YCIComponent之间的双向消息转发
//      因为Model和View之间的生存周期都不一样
//      比如Package中的物品和界面中的格子
//      相互引用容易出现野指针
//      支持[] 下标操作
//
//
//               YCIModel                     Slots                       YCIComponent
//                                        -------------
//                         <=============|    Event    |=============>
//										 |-------------|
//						   <=============|    Event    |=============>
//										 |-------------|
//						   <=============|    Event    |=============>
//										 |-------------|
//						   <=============|    Event    |=============>
//										 |-------------|
//						   <=============|    Event    |=============>
//										 |-------------|
//						   <=============|    Event    |=============>
//										 |-------------|
//						   <=============|    Event    |=============>
//										 |-------------|
//  					   <=============|    Event    |=============>
//										 |-------------|
//                         <=============|    Event    |=============>
//                                        -------------
//   1. YCIMode到YCIComponent 支持3种 Event : 
//         出现 调用 ->YCIComponent update model != NULL ???
//         消失 调用 ->YCIComponent update model == NULL ???
//         变化 调用 ->YCIComponent update model != NULL ???
//
//   2. YCIComponent 到 YCIModel 支持 1 种
//         变化 调用 ->YCIModel update ???
//

struct YCEvent;

class YCDList;
class YCIModel;
class YCIComponent;

class YCUIMediator
{
public:

	YCUIMediator(unsigned int length);

	virtual ~YCUIMediator(void);

	//
	// 增大槽位
	//
	void resize(unsigned int largerLength);

	//
	// 设置某个槽位的Model   M->V
	//
	void setModel(unsigned int slot, YCIModel* model);

	//
	// 获取某个槽位的Model   V->M
	//
	YCIModel* getModel(unsigned int slot);

	//
	// 移除某个槽位的Model   M->V
	//
	void removeModel(unsigned int slot);

	//
	// 界面打开后添加绑定 V->M
	//
	void bindSlot(unsigned int slot, YCIComponent* component);

	//
	// 界面关闭时解除绑定 V->M
	//
	void unbind(YCIComponent* component);

	//
	// 更新某个槽位的Model   M->V
	//
	void modelUpdated(unsigned int slot);
	
	// 
	// 函数：updateModel(unsigned int slot, const YCEvent& ev)
	//
	// 界面对Model的操作请求 V->M
	//
	virtual void updateModel(unsigned int slot, const YCEvent& ev) = 0;

private:

	struct Slot_T;

	//
	// 创建指定长度槽位
	//
	Slot_T** createSlots(unsigned int size);

	//
	// 释放所有槽位
	//
	void releaseSlots();

private:

	struct Slot_T {
		YCIModel* model; 
		YCDList* monitors;
	};

	unsigned int myLength;

	Slot_T** mySlots;
};

#endif

