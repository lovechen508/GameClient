#ifndef _INCLUDE_YCUIMEDIATOR_H_
#define _INCLUDE_YCUIMEDIATOR_H_

//
// YCUIMediator ���н���
//
// ˵��������YCIModel��YCIComponent֮���˫����Ϣת��
//      ��ΪModel��View֮����������ڶ���һ��
//      ����Package�е���Ʒ�ͽ����еĸ���
//      �໥�������׳���Ұָ��
//      ֧��[] �±����
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
//   1. YCIMode��YCIComponent ֧��3�� Event : 
//         ���� ���� ->YCIComponent update model != NULL ???
//         ��ʧ ���� ->YCIComponent update model == NULL ???
//         �仯 ���� ->YCIComponent update model != NULL ???
//
//   2. YCIComponent �� YCIModel ֧�� 1 ��
//         �仯 ���� ->YCIModel update ???
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
	// �����λ
	//
	void resize(unsigned int largerLength);

	//
	// ����ĳ����λ��Model   M->V
	//
	void setModel(unsigned int slot, YCIModel* model);

	//
	// ��ȡĳ����λ��Model   V->M
	//
	YCIModel* getModel(unsigned int slot);

	//
	// �Ƴ�ĳ����λ��Model   M->V
	//
	void removeModel(unsigned int slot);

	//
	// ����򿪺���Ӱ� V->M
	//
	void bindSlot(unsigned int slot, YCIComponent* component);

	//
	// ����ر�ʱ����� V->M
	//
	void unbind(YCIComponent* component);

	//
	// ����ĳ����λ��Model   M->V
	//
	void modelUpdated(unsigned int slot);
	
	// 
	// ������updateModel(unsigned int slot, const YCEvent& ev)
	//
	// �����Model�Ĳ������� V->M
	//
	virtual void updateModel(unsigned int slot, const YCEvent& ev) = 0;

private:

	struct Slot_T;

	//
	// ����ָ�����Ȳ�λ
	//
	Slot_T** createSlots(unsigned int size);

	//
	// �ͷ����в�λ
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

