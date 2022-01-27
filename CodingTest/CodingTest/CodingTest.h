#pragma once

#include <map>
#include <list>
#include <vector>

class CodingTest
{
public:
	enum class TestCase
	{
		TestCase_01,
		TestCase_02,
		TestCase_03,
		TestCase_04,
		TestCase_05,
	};

public:
	CodingTest() {};
	virtual ~CodingTest() {};

	virtual bool Test_Logic(TestCase _Test_Case) abstract;
};

