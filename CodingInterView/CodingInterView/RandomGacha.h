#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <random>

struct Weapon
{
	// 레벨
	int m_LVL = 0;

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

	void Reset()
	{
		m_STR = 0;
		m_DEX = 0;
		m_INT = 0;
		m_LUK = 0;
		m_DEF = 0;
		m_All = 0;
		m_JMP = 0;
	};
};

class RandomGacha
{
private:
	// 추가 옵션에 들어 갈 수 있는 스탯들
	std::vector<std::string> m_Option_Kind =
	{
		"STR", "DEX", "INT", "LUK",
		"STR+DEX", "STR+INT", "STR+LUK", "DEX+INT", "DEX+LUK", "INT+LUK",
		"DEF", "ALL", "JMP"
	};
	// 중복 옵션 체크
	std::map<std::string, int> m_Duplicate;

	// 강화수치, 해당 데이터
	std::map<int, std::discrete_distribution<>> m_Starforce_Table;
	// 강환불, 해당 데이터
	std::map<int, std::discrete_distribution<>> m_Rebirth_Table;
	// 추옵에 대한 데이터 (레벨, 단일/이중)
	std::map<int, std::pair<int, int>> m_Additional_Table;
	// 무기 만들기
	std::queue<Weapon*> m_Weapon_List;
	// 강화된 최종 무기
	std::vector<int> m_After_Weapon;
	// 강환불 시, 추옵을 고를 무기
	Weapon m_Weapon;
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
	// 무기 생성
	void MakeWeapon(int _Count);
	// 스타포스 강화!
	void ReinforceStarforce();
	// 강환불 / 영환불 시도!
	void ReinforceRebirth();

private:
	// 무기 옵션 출력
	void PrintWeapon();
	// 무기 옵션 달기
	void SetOption(std::mt19937& _gen);
};

