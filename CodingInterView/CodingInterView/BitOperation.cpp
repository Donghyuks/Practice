#include "BitOperation.h"

int BitOperation::GetBit(int _index)
{
	// ���� ����Ʈ index�� ����
	int _And_Compare = 1 << _index;

	// �� �� ���� ���� & ����
	_And_Compare &= m_Standard;

	return _And_Compare > 0 ? 1 : 0;
}

int BitOperation::SetBit(int _index)
{
	// ��� ���� �����ϱ� ���� ����
	int _Result = 0;

	// ���� ����Ʈ index�� ����
	int _Or_Compare = 1 << _index;

	// �� �� ���� ���� | ����
	_Result = _Or_Compare | m_Standard;

	return _Result;
}

int BitOperation::ClearBit(int _index)
{
	// ��� ���� �����ϱ� ���� ����
	int _Result = 0;

	// index ��ġ�� �����ϰ� ��� 1�� �ʱ�ȭ�� �Ѵ�.
	int _And_Compare = -1 - (1 << _index);

	// �� �� ���� ���� & ������ �����ϸ� �ش� ��Ʈ�� 0���� ����
	_Result = _And_Compare & m_Standard;

	return _Result;
}

int BitOperation::ClearLBit(int _index)
{
	// ��� ���� �����ϱ� ���� ����
	int _Result = 0;

	// -1 (1111111...) ���� index +1 ��ŭ ����Ʈ�� ���� �� not�� ���ش�.
	int _And_Compare = ~(-1 << (_index + 1));
	
	// �� �� and ������ ����
	_Result = m_Standard & _And_Compare;

	return _Result;
}

int BitOperation::ClearRBit(int _index)
{
	// ��� ���� �����ϱ� ���� ����
	int _Result = 0;

	// -1 (1111111...) ���� index ��ŭ ����Ʈ�� ����.
	int _And_Compare = -1 << (_index + 1);

	// �� �� and ������ ����
	_Result = m_Standard & _And_Compare;

	return _Result;
}

int BitOperation::UpdateBit(int _index, bool _val)
{
	int _Result = 0;

	// �����ϰ��� �ϴ� ���� 0 �϶� -> ������ ���� 1�� �ٲٰ� & ����
	if (_val == false)
	{
		int _And_Compare = ~(1 << _index);

		_Result = m_Standard & _And_Compare;
	}
	// �����ϰ��� �ϴ� ���� 1 �϶� -> ������ ���� 0���� �ٲٰ� | ����
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
