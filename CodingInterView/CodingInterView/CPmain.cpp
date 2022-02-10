#include "ConstPointer.h"

int main()
{
	ConstPointer CP;

	int _Test = 20;
	int _Test2 = 30;

	// Const ������ ����� �������� �ص� �ش� ������ ���ο� ���� �ٲ��� ����.
	CP.SwapConstPointer(&_Test);

	const int* _Test_Ptr = &_Test;

	// ���� �����Ͱ� ����Ű�� ���� �ٲٰ� �ʹٸ�? �ٲ� �� �ִ�! 
	// ���1
	CP.SwapPointer(&_Test_Ptr);
	// ���2
	_Test_Ptr = &_Test2;

	// �̷��͵� �ȴ�..
	const int* const* const _Test_Abs_Value = &_Test_Ptr;

	// ������ Const ���� �� ������ ���� �ٲ� �� ������, �������� ��ȯ�� �ȵ�!
	CP.SwapPointerConstValue(&_Test);

	return 0;
}