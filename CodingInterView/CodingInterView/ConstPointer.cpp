#include "ConstPointer.h"

void ConstPointer::SwapPointer(const int** _val)
{
	*_val = &_Test_Val;
}

void ConstPointer::SwapConstPointer(const int* _val)
{
	_val = &_Test_Val;		// ����

	// ������ ������ const int* _val �̶�� �Ű������� ���� �ٲ�� ������ ����
	// _val �� �ҷ����� ���� ������ �ּ��� �������� ������ ����!
}

void ConstPointer::SwapConstPointerValue(const int* _val)
{
	// �Ұ���
	// *_val = _Test_Val;
}

void ConstPointer::SwapPointerConst(int* const _val)
{
	// �Ұ���
	//_val = &_Test_Val;
}

void ConstPointer::SwapPointerConstValue(int* const _val)
{
	*_val = _Test_Val;	// ����
}
