#include "ConstPointer.h"

void ConstPointer::SwapPointer(const int** _val)
{
	*_val = &_Test_Val;
}

void ConstPointer::SwapConstPointer(const int* _val)
{
	_val = &_Test_Val;		// 가능

	// 하지만 실제로 const int* _val 이라는 매개변수의 값이 바뀌는 것이지 실제
	// _val 을 불러오기 위해 쓰였던 주소의 참조값은 변하지 않음!
}

void ConstPointer::SwapConstPointerValue(const int* _val)
{
	// 불가능
	// *_val = _Test_Val;
}

void ConstPointer::SwapPointerConst(int* const _val)
{
	// 불가능
	//_val = &_Test_Val;
}

void ConstPointer::SwapPointerConstValue(int* const _val)
{
	*_val = _Test_Val;	// 가능
}
