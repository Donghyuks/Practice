
#include "2_NetWork.h"

int main()
{
	NetWork* Current_Test = new NetWork();

	Current_Test->Input_TestCase(CodingTest::TestCase::TestCase_01, 3, { {1,1,0},{1,1,0},{0,0,1} }, 2);
	Current_Test->Input_TestCase(CodingTest::TestCase::TestCase_02, 3, { {1,1,0},{1,1,1},{0,1,1} }, 1);

	Current_Test->Test_Logic(CodingTest::TestCase::TestCase_01);

	while (true)
	{

	}
}