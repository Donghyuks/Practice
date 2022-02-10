#pragma once


class ConstPointer
{
	int _Test_Val = 0;

public:
	void SwapPointer(const int** _val);

	void SwapConstPointer(const int* _val);
	void SwapConstPointerValue(const int*  _val);

	void SwapPointerConst(int* const _val);
	void SwapPointerConstValue(int* const  _val);




};

