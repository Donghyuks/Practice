#pragma once

#include "CodingTest.h"

class NetWork : public CodingTest
{
public:
	NetWork();
	virtual ~NetWork();

	struct Input
	{
		int n = 0;
		std::vector<std::vector<int>> computers;
	};

	struct Output
	{
		int answer = 0;
	};

	struct TestData
	{
		Input In;
		Output Out;
	};

private:
	std::map<TestCase, TestData> m_Test;

public:
	void Input_TestCase(TestCase _Test_Case, int _n, std::vector<std::vector<int>> _computers, int _answer);
	virtual bool Test_Logic(TestCase _Test_Case);


};