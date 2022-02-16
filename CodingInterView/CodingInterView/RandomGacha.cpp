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

		// ù���� ���
		if (!m_Is_Once)
		{
			// utf8(bom) ���Ŀ��� ó�� 3�ܾ �����ϰ� ����
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
	// ��Ÿ���� Ȯ�� ���̺� �б�
	ReadDoubleCSV(_Path);

	// ��Ÿ���� ������ �־����.
	for (int i = 0; i < m_CSV_Data.size() / m_ColumCount; i++)
	{
		// ��ȭ ��ġ�� ������ Ȯ���� ���� �����Ѵ�.
		m_Starforce_Table.insert({ i, {m_CSV_Data[i * m_ColumCount] , m_CSV_Data[i * m_ColumCount + 1], 
			m_CSV_Data[i * m_ColumCount + 2], m_CSV_Data[i * m_ColumCount + 3]} });
	}
}

void RandomGacha::SetRebirthFireOption(const std::string& _Path)
{
	// ��ȯ�� Ȯ�� �б�
	ReadDoubleCSV(_Path);

	// ��ȯ�� ������ �־����.
	for (int i = 0; i < m_CSV_Data.size() / m_ColumCount; i++)
	{
		// ��ȭ ��ġ�� ������ Ȯ���� ���� �����Ѵ�.
		m_Rebirth_Table.insert({ i, {m_CSV_Data[i * m_ColumCount] , m_CSV_Data[i * m_ColumCount + 1], m_CSV_Data[i * m_ColumCount + 2],
			m_CSV_Data[i * m_ColumCount + 3], m_CSV_Data[i * m_ColumCount + 4]} });
	}
}

void RandomGacha::SetAdditionalOption(const std::string& _Path)
{
	// �߰� �ɼ� �б�
	ReadDoubleCSV(_Path);

	// ù �ٿ��� �������� ������ ����.
	for (int i = 0; i < m_ColumCount; i++)
	{
		m_Additional_Table.insert({ (int)m_CSV_Data[i], {0,0} });
	}

	// ��° �ٿ��� �����ۿ� ���� ���� / ���� �߰� �ɼ� ��ġ
	for (int i = m_ColumCount; i < m_CSV_Data.size();)
	{
		int _Current_Level = (i - m_ColumCount) / 2;
		_Current_Level = m_CSV_Data[_Current_Level];
		// �ش� ������ �����ۿ� ���� ���� / ���� ��ġ
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

	std::cout << " ���Ⱑ " << m_Weapon_List.size() << " �� ���� �Ǿ����ϴ�." << std::endl;
}

void RandomGacha::ReinforceStarforce()
{
	m_After_Weapon.resize(25, 0);

	// ���� ���׷�����
	std::random_device rd;
	std::mt19937 gen(rd());
	
	// ���� ��Ÿ����
	int _Current_Starforce = 0;
	// ���� �õ� Ƚ��
	int _Reinforce_Count = 0;
	// ���� ���� �� ����
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

	std::cout << " ==== ���� ��ȭ ��� ==== " << std::endl;
	std::cout << " ���� ���� ���� : " << _Current_Weapon_Count << "    �� �õ� Ƚ�� : " << _Reinforce_Count << " ��" << std::endl;

	for (int i = 1; i < m_After_Weapon.size() - 1; i++)
	{
		std::cout << " " << i << " �� ���� �ı�: " << m_After_Weapon[i] << " �� " << std::endl;
	}

	std::cout << " ������ 24�� ����: " << m_After_Weapon[24] << " �� " << std::endl;
}

void RandomGacha::ReinforceRebirth()
{
	bool _Is_End = false;
	int _Weapon_Level = 0;	// ���� ����
	int _Option_Count = 0;	// �ɼ� ����

	// ���� ���׷�����
	std::random_device rd;
	std::mt19937 gen(rd());

	// ��ȭ�� ���� ���� �޾ƿ���.
	std::cin >> _Weapon_Level;
	m_Weapon.m_LVL = _Weapon_Level;
	// ���� �ɼ� ���
	SetOption(gen);

	while (!_Is_End)
	{
		// RŰ�� �缳��
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
	std::cout << " ����\t:" << m_Weapon.m_DEF << std::endl;
	std::cout << " �ý���\t:" << m_Weapon.m_All << std::endl;
	std::cout << " ������\t:" << m_Weapon.m_JMP << std::endl;
	std::cout << "=============================" << std::endl;
	std::cout << " �õ� Ƚ��\t:" << m_Weapon.m_Try_Rebirth_Count << std::endl;
	std::cout << "=============================" << std::endl;
	int _Count = 1;
	for (auto _Option_Stat : m_Duplicate)
	{
		std::cout << " �ɼ� " << _Count << "\t:" << _Option_Stat.first << " + " << _Option_Stat.second << std::endl;
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
	// �̱ۿɼ�
	int _Single = 0;
	// ���ɼ�
	int _Duel = 0;
	std::uniform_int_distribution<> _Range(0, m_Option_Kind.size() - 1);

	// ȯ�� Ƚ�� �߰�
	m_Weapon.m_Try_Rebirth_Count++;

	_Single = m_Additional_Table[m_Weapon.m_LVL].first;
	_Duel = m_Additional_Table[m_Weapon.m_LVL].second;

	// �߰� �ɼ��� 4��!
	for (int i = 0; i < 4; i++)
	{
		// ��ȯ�� ���� 1~5 ������� ����.
		_Rebirth_Rank = m_Rebirth_Table[0](_gen);
		_Rebirth_Rank++;

		// ���������ΰ�� + 2
		if (m_Weapon.m_LVL > 150)
		{
			_Rebirth_Rank += 2;
		}

		while (true)
		{
			// �ɼ� �߿� � �ɼ��� �� ���ΰ�?
			_Random_Option = _Range(_gen);

			auto _Result = m_Duplicate.find(m_Option_Kind[_Random_Option]);

			// ���� ����� ���ٸ�
			if (_Result == m_Duplicate.end())
			{
				// �ش� �ɼ� ��ȯ
				break;
			}
		}

		// �ش� �ɼǿ� ���� ��ġ ����
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
