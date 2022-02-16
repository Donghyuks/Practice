#include "RandomGacha.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>

void RandomGacha::ReadDoubleCSV(const std::string& _Path)
{
	m_CSV_Data.clear();
	std::ifstream _file(_Path);
	if (_file.fail()) { _file.close(); return; }

	std::string _cell;
	std::string _line;

	m_ColumCount = 0;
	m_Is_Once = false;

	while (std::getline(_file, _line))
	{
		std::stringstream _lineStream(_line);

		// 첫줄인 경우
		if (!m_Is_Once)
		{
			// utf8(bom) 형식에서 처음 3단어를 제외하고 읽음
			char utf8bom[3] = {};
			std::streamsize _Size = _lineStream.readsome(utf8bom, 3);

			if (3 == _Size)
			{
				if ((char)0xEF == utf8bom[0] && (char)0xBB == utf8bom[1] && (char)0xBF == utf8bom[2])
				{
					_lineStream.seekg(3);
				}
			}
		}

		while (std::getline(_lineStream, _cell, ','))
		{
			if (_cell == "") break;
			if (!m_Is_Once) m_ColumCount++;
			m_CSV_Data.push_back(stod(_cell));
		}

		m_Is_Once = true;
	}
}

void RandomGacha::SetStarForce(const std::string& _Path)
{
	// 스타포스 확률 테이블 읽기
	ReadDoubleCSV(_Path);

	// 스타포스 데이터 넣어놓기.
	for (int i = 0; i < m_CSV_Data.size() / m_ColumCount; i++)
	{
		// 강화 수치와 각각의 확률을 같이 저장한다.
		m_Starforce_Table.insert({ i, {m_CSV_Data[i * m_ColumCount] , m_CSV_Data[i * m_ColumCount + 1], 
			m_CSV_Data[i * m_ColumCount + 2], m_CSV_Data[i * m_ColumCount + 3]} });
	}
}

void RandomGacha::SetRebirthFireOption(const std::string& _Path)
{
	// 강환불 확률 읽기
	ReadDoubleCSV(_Path);

	// 강환불 데이터 넣어놓기.
	for (int i = 0; i < m_CSV_Data.size() / m_ColumCount; i++)
	{
		// 강화 수치와 각각의 확률을 같이 저장한다.
		m_Rebirth_Table.insert({ i, {m_CSV_Data[i * m_ColumCount] , m_CSV_Data[i * m_ColumCount + 1], m_CSV_Data[i * m_ColumCount + 2],
			m_CSV_Data[i * m_ColumCount + 3], m_CSV_Data[i * m_ColumCount + 4]} });
	}
}

void RandomGacha::SetAdditionalOption(const std::string& _Path)
{
	// 추가 옵션 읽기
	ReadDoubleCSV(_Path);

	// 첫 줄에는 아이템의 레벨이 들어옴.
	for (int i = 0; i < m_ColumCount; i++)
	{
		m_Additional_Table.insert({ (int)m_CSV_Data[i], {0,0} });
	}

	// 둘째 줄에는 아이템에 대한 단일 / 이중 추가 옵션 수치
	for (int i = m_ColumCount; i < m_CSV_Data.size();)
	{
		int _Current_Level = (i - m_ColumCount) / 2;
		_Current_Level = m_CSV_Data[_Current_Level];
		// 해당 레벨의 아이템에 대한 단일 / 이중 수치
		m_Additional_Table[_Current_Level].first = (int)m_CSV_Data[i];
		m_Additional_Table[_Current_Level].second = (int)m_CSV_Data[i + 1];

		i += 2;
	}
}

void RandomGacha::MakeWeapon(int _Count)
{
	for (int i = 0; i < _Count; i++)
	{
		m_Weapon_List.push(new Weapon);
	}

	std::cout << " 무기가 " << m_Weapon_List.size() << " 개 생성 되었습니다." << std::endl;
}

void RandomGacha::ReinforceStarforce()
{
	m_After_Weapon.resize(25, 0);

	// 랜덤 제네레이터
	std::random_device rd;
	std::mt19937 gen(rd());
	
	// 현재 스타포스
	int _Current_Starforce = 0;
	// 무기 시도 횟수
	int _Reinforce_Count = 0;
	// 현재 무기 총 개수
	size_t _Current_Weapon_Count = m_Weapon_List.size();

	while (!m_Weapon_List.empty())
	{
		Weapon* _Current_Weapon = m_Weapon_List.front();
		m_Weapon_List.pop();

		_Reinforce_Count++;
		_Current_Weapon->m_Try_Starforce_Count++;
		_Current_Starforce = _Current_Weapon->m_Starforce;
		int _Result = m_Starforce_Table[_Current_Starforce](gen);

		//std::cout << "min() == " << m_Starforce_Table[_Current_Starforce].min() << std::endl;
		//std::cout << "max() == " << m_Starforce_Table[_Current_Starforce].max() << std::endl;
		//std::cout << "probabilities (value: probability):" << std::endl;
		//std::vector<double> p = m_Starforce_Table[_Current_Starforce].probabilities();
		//int counter = 0;
		//for (const auto& n : p)
		//{
		//	std::cout << counter << " : " << n << std::endl;
		//	++counter;
		//}
		//std::cout << std::endl;

		// fail
		if (_Result == 0)
		{
			m_Weapon_List.push(_Current_Weapon);
		}
		// success
		else if (_Result == 1)
		{
			_Current_Weapon->m_Starforce++;

			if (_Current_Weapon->m_Starforce == 24)
			{
				m_After_Weapon[_Current_Weapon->m_Starforce]++;
				continue;
			}

			m_Weapon_List.push(_Current_Weapon);
		}
		// destroy
		else if (_Result == 2)
		{
			m_After_Weapon[_Current_Weapon->m_Starforce]++;
		}
		// keep
		else if (_Result == 3)
		{
			m_Weapon_List.push(_Current_Weapon);
		}
	}

	std::cout << " ==== 무기 강화 결과 ==== " << std::endl;
	std::cout << " 시작 무기 갯수 : " << _Current_Weapon_Count << "    총 시도 횟수 : " << _Reinforce_Count << " 번" << std::endl;

	for (int i = 1; i < m_After_Weapon.size() - 1; i++)
	{
		std::cout << " " << i << " 강 에서 파괴: " << m_After_Weapon[i] << " 개 " << std::endl;
	}

	std::cout << " 성공한 24강 갯수: " << m_After_Weapon[24] << " 개 " << std::endl;
}

void RandomGacha::ReinforceRebirth()
{
	bool _Is_End = false;
	int _Weapon_Level = 0;	// 무기 레벨
	int _Option_Count = 0;	// 옵션 개수

	// 랜덤 제네레이터
	std::random_device rd;
	std::mt19937 gen(rd());

	// 강화할 무기 레벨 받아오기.
	std::cin >> _Weapon_Level;
	m_Weapon.m_LVL = _Weapon_Level;
	// 무기 옵션 출력
	SetOption(gen);

	while (!_Is_End)
	{
		// R키로 재설정
		if (GetAsyncKeyState('R') & 0x8000)
		{
			SetOption(gen);
			Sleep(1000);
		}
	}
}

void RandomGacha::PrintWeapon()
{
	system("cls");
	std::cout << "=============================" << std::endl;
	std::cout << " Level\t:" << m_Weapon.m_LVL << std::endl;
	std::cout << " STR\t:" << m_Weapon.m_STR << std::endl;
	std::cout << " DEX\t:" << m_Weapon.m_DEX << std::endl;
	std::cout << " INT\t:" << m_Weapon.m_INT << std::endl;
	std::cout << " LUK\t:" << m_Weapon.m_LUK << std::endl;
	std::cout << " 방어력\t:" << m_Weapon.m_DEF << std::endl;
	std::cout << " 올스탯\t:" << m_Weapon.m_All << std::endl;
	std::cout << " 점프력\t:" << m_Weapon.m_JMP << std::endl;
	std::cout << "=============================" << std::endl;
	std::cout << " 시도 횟수\t:" << m_Weapon.m_Try_Rebirth_Count << std::endl;
	std::cout << "=============================" << std::endl;
	int _Count = 1;
	for (auto _Option_Stat : m_Duplicate)
	{
		std::cout << " 옵션 " << _Count << "\t:" << _Option_Stat.first << " + " << _Option_Stat.second << std::endl;
		_Count++;
	}
	std::cout << "=============================" << std::endl;
}

void RandomGacha::SetOption(std::mt19937& _gen)
{
	m_Duplicate.clear();
	m_Weapon.Reset();
	int _Rebirth_Rank = 0;
	int _Random_Option = 0;
	// 싱글옵션
	int _Single = 0;
	// 듀얼옵션
	int _Duel = 0;
	std::uniform_int_distribution<> _Range(0, m_Option_Kind.size() - 1);

	// 환불 횟수 추가
	m_Weapon.m_Try_Rebirth_Count++;

	_Single = m_Additional_Table[m_Weapon.m_LVL].first;
	_Duel = m_Additional_Table[m_Weapon.m_LVL].second;

	// 추가 옵션은 4개!
	for (int i = 0; i < 4; i++)
	{
		// 강환불 먼저 1~5 등급인지 결정.
		_Rebirth_Rank = m_Rebirth_Table[0](_gen);
		_Rebirth_Rank++;

		// 보스무기인경우 + 2
		if (m_Weapon.m_LVL > 150)
		{
			_Rebirth_Rank += 2;
		}

		while (true)
		{
			// 옵션 중에 어떤 옵션이 들어갈 것인가?
			_Random_Option = _Range(_gen);

			auto _Result = m_Duplicate.find(m_Option_Kind[_Random_Option]);

			// 만약 결과가 없다면
			if (_Result == m_Duplicate.end())
			{
				// 해당 옵션 반환
				break;
			}
		}

		// 해당 옵션에 대한 수치 적용
		if (m_Option_Kind[_Random_Option] == "STR")
		{
			m_Weapon.m_STR += _Single * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Single * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "DEX")
		{
			m_Weapon.m_DEX += _Single * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Single * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "INT")
		{
			m_Weapon.m_INT += _Single * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Single * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "LUK")
		{
			m_Weapon.m_LUK += _Single * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Single * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "STR+DEX")
		{
			m_Weapon.m_STR += _Duel * _Rebirth_Rank;
			m_Weapon.m_DEX += _Duel * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Duel * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "STR+INT")
		{
			m_Weapon.m_STR += _Duel * _Rebirth_Rank;
			m_Weapon.m_INT += _Duel * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Duel * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "STR+LUK")
		{
			m_Weapon.m_STR += _Duel * _Rebirth_Rank;
			m_Weapon.m_LUK += _Duel * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Duel * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "DEX+INT")
		{
			m_Weapon.m_DEX += _Duel * _Rebirth_Rank;
			m_Weapon.m_INT += _Duel * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Duel * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "DEX+LUK")
		{
			m_Weapon.m_DEX += _Duel * _Rebirth_Rank;
			m_Weapon.m_LUK += _Duel * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Duel * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "INT+LUK")
		{
			m_Weapon.m_INT += _Duel * _Rebirth_Rank;
			m_Weapon.m_LUK += _Duel * _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Duel * _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "DEF")
		{
			m_Weapon.m_DEF += _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "ALL")
		{
			m_Weapon.m_All += _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Rebirth_Rank });
		}
		else if (m_Option_Kind[_Random_Option] == "JMP")
		{
			m_Weapon.m_JMP += _Rebirth_Rank;
			m_Duplicate.insert({ m_Option_Kind[_Random_Option], _Rebirth_Rank });
		}
	}

	PrintWeapon();
}
