#include "ConstPointer.h"

int main()
{
	ConstPointer CP;

	int _Test = 20;
	int _Test2 = 30;

	// Const 포인터 선언시 무슨짓을 해도 해당 포인터 내부에 값은 바뀌지 않음.
	CP.SwapConstPointer(&_Test);

	const int* _Test_Ptr = &_Test;

	// 만약 포인터가 가리키는 곳을 바꾸고 싶다면? 바꿀 수 있다! 
	// 방법1
	CP.SwapPointer(&_Test_Ptr);
	// 방법2
	_Test_Ptr = &_Test2;

	// 이런것도 된다..
	const int* const* const _Test_Abs_Value = &_Test_Ptr;

	// 포인터 Const 선언 시 내부의 값은 바꿀 수 있으나, 포인터의 교환은 안됨!
	CP.SwapPointerConstValue(&_Test);

	return 0;
}