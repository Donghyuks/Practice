#pragma once

#include <string>
#include <vector>

class RandomGacha
{
private:
	std::vector<double> m_CSV_Data;
	int m_ColumCount = 0;

private:
	// double 형으로 csv파일 읽기
	void ReadDoubleCSV(const std::string& _Path);

public:
	// 스타포스 확률 csv 파일
	void SetStarForce(const std::string& _Path);
	// 강환불 확률 csv 파일
	void SetRebirthFireOption(const std::string& _Path);
	// 방어구 추옵 확률 csv 파일
	void SetAdditionalOption(const std::string& _Path);
	// 방어구 추옵(1로 고정인 확률) 추가
	void AddFixed_AdditionalOption(const std::string& _Option_Name);

};

