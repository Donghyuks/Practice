#include "RandomGacha.h"
#include <iostream>
#include <sstream>
#include <fstream>

void RandomGacha::ReadDoubleCSV(const std::string& _Path)
{
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
		m_Starforce_Table.insert({ i, {m_CSV_Data[i * m_ColumCount] , m_CSV_Data[i * m_ColumCount + 1], m_CSV_Data[i * m_ColumCount + 2], m_CSV_Data[i * m_ColumCount + 3]} });
	}
}

void RandomGacha::SetRebirthFireOption(const std::string& _Path)
{

}

void RandomGacha::SetAdditionalOption(const std::string& _Path)
{

}

void RandomGacha::AddFixed_AdditionalOption(const std::string& _Option_Name)
{

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
	std::cout << " 시작 무기 개수 : " << _Current_Weapon_Count << "    총 시도 횟수 : " << _Reinforce_Count << " 번" << std::endl;

	for (int i = 1; i < m_After_Weapon.size(); i++)
	{
		std::cout << i << " 강 : " << m_After_Weapon[i] << " 개 " << std::endl;
	}
}
