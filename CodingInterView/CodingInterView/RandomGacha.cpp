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

	for (int i = 1; i < m_After_Weapon.size(); i++)
	{
		std::cout << i << " �� : " << m_After_Weapon[i] << " �� " << std::endl;
	}
}
