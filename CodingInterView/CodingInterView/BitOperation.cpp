#include "BitOperation.h"

int BitOperation::GetBit(int _index)
{
	// 왼쪽 쉬프트 index번 진행
	int _And_Compare = 1 << _index;

	// 그 후 원본 값과 & 연산
	_And_Compare &= m_Standard;

	return _And_Compare > 0 ? 1 : 0;
}

int BitOperation::SetBit(int _index)
{
	// 결과 값을 저장하기 위한 변수
	int _Result = 0;

	// 왼쪽 쉬프트 index번 진행
	int _Or_Compare = 1 << _index;

	// 그 후 원본 값과 | 연산
	_Result = _Or_Compare | m_Standard;

	return _Result;
}

int BitOperation::ClearBit(int _index)
{
	// 결과 값을 저장하기 위한 변수
	int _Result = 0;

	// index 위치를 제외하고 모두 1로 초기화를 한다.
	int _And_Compare = -1 - (1 << _index);

	// 그 후 원본 값과 & 연산을 진행하면 해당 비트만 0으로 셋팅
	_Result = _And_Compare & m_Standard;

	return _Result;
}

int BitOperation::ClearLBit(int _index)
{
	// 결과 값을 저장하기 위한 변수
	int _Result = 0;

	// -1 (1111111...) 에서 index +1 만큼 쉬프트를 진행 후 not을 해준다.
	int _And_Compare = ~(-1 << (_index + 1));
	
	// 그 후 and 연산을 진행
	_Result = m_Standard & _And_Compare;

	return _Result;
}

int BitOperation::ClearRBit(int _index)
{
	// 결과 값을 저장하기 위한 변수
	int _Result = 0;

	// -1 (1111111...) 에서 index 만큼 쉬프트를 진행.
	int _And_Compare = -1 << (_index + 1);

	// 그 후 and 연산을 진행
	_Result = m_Standard & _And_Compare;

	return _Result;
}

int BitOperation::UpdateBit(int _index, bool _val)
{
	int _Result = 0;

	// 셋팅하고자 하는 값이 0 일때 -> 나머지 값을 1로 바꾸고 & 연산
	if (_val == false)
	{
		int _And_Compare = ~(1 << _index);

		_Result = m_Standard & _And_Compare;
	}
	// 셋팅하고자 하는 값이 1 일때 -> 나머지 값을 0으로 바꾸고 | 연산
	else
	{
		int _Or_Compare = 1 << _index;

		_Result = m_Standard | _Or_Compare;
	}

	return _Result;
}

void BitOperation::SwapInt(int& _val1, int& _val2)
{
	if (_val1 != _val2)
	{
		_val1 ^= _val2;
		_val2 ^= _val1;
		_val1 ^= _val2;
	}
}
