#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <random>

struct Weapon
{
	// 스타포스
	int m_Starforce = 0;

	// 주 스텟
	int m_STR = 0;
	int m_DEX = 0;
	int m_INT = 0;
	int m_LUK = 0;
	int m_DEF = 0;
	int m_All = 0;
	int m_JMP = 0;

	// 시도한 횟수
	int m_Try_Starforce_Count = 0;
	int m_Try_Rebirth_Count = 0;
};
//
//struct StartForceTable
//{
//	double m_Fail = 0.f;
//	double m_Success = 0.f;
//	double m_Destroy = 0.f;
//	double m_Keep = 0.f;
//};

class RandomGacha
{
private:
	// 강화수치, 해당 데이터
	std::map<int, std::discrete_distribution<>> m_Starforce_Table;
	// 무기 만들기
	std::queue<Weapon*> m_Weapon_List;
	// 강화된 최종 무기
	std::vector<int> m_After_Weapon;

	std::vector<double> m_CSV_Data;
	int m_ColumCount = 0;
	bool m_Is_Once = false;

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
	// 무기 생성
	void MakeWeapon(int _Count);
	// 스타포스 강화!
	void ReinforceStarforce();
};

