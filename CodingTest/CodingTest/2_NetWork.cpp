#include "2_NetWork.h"

NetWork::NetWork()
{
	
}

NetWork::~NetWork()
{

}


void NetWork::Input_TestCase(TestCase _Test_Case, int _n, std::vector<std::vector<int>> _computers, int _answer)
{
	Input _In;
	Output _Out;

	_In.n = _n;
	_In.computers = _computers;

	_Out.answer = _answer;

	m_Test.insert({ _Test_Case, {_In, _Out} });
}

bool NetWork::Test_Logic(TestCase _Test_Case)
{
	int answer = 0;

	int n = m_Test[_Test_Case].In.n;
	std::vector<std::vector<int>> computers = m_Test[_Test_Case].In.computers;






	if (answer == m_Test[_Test_Case].Out.answer) { return true; }
	else { return false; }
}
