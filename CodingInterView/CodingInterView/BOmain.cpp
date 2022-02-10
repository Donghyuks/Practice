#include "BitOperation.h"

int main()
{
	BitOperation BO(0b10101001);

	int _Get_Bit = BO.GetBit(3);
	
	int _Set_Bit = BO.SetBit(2);

	int _Clear_Bit = BO.ClearBit(3);

	int _LClear_Bit = BO.ClearLBit(3);

	int _RClear_Bit = BO.ClearRBit(3);

	int _Update_Bit = BO.UpdateBit(3, 0);
	
	int _Update_Bit2 = BO.UpdateBit(2, 1);

	int x = 10;
	int y = 5;

	BO.SwapInt(x, y);

	return 0;
}