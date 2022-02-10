#pragma once

// Update Bit

// Clear Right Bit


class BitOperation
{
private:
	int m_Standard = 0;

public:
	BitOperation(int _Standard) { m_Standard = _Standard; };

	// Get Bit
	int GetBit(int _index);
	// Set Bit
	int SetBit(int _index);
	// Clear Bit
	int ClearBit(int _index);
	// Clear Left Bit
	int ClearLBit(int _index);
	// Clear Right Bit
	int ClearRBit(int _index);
	// Update Bit
	int UpdateBit(int _index, bool _val);
	
	// INT Swap
	void SwapInt(int& _val1, int& _val2);
};