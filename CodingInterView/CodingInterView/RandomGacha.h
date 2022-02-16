#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <random>

struct Weapon
{
	// ����
	int m_LVL = 0;

	// ��Ÿ����
	int m_Starforce = 0;

	// �� ����
	int m_STR = 0;
	int m_DEX = 0;
	int m_INT = 0;
	int m_LUK = 0;
	int m_DEF = 0;
	int m_All = 0;
	int m_JMP = 0;

	// �õ��� Ƚ��
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
	// �߰� �ɼǿ� ��� �� �� �ִ� ���ȵ�
	std::vector<std::string> m_Option_Kind =
	{
		"STR", "DEX", "INT", "LUK",
		"STR+DEX", "STR+INT", "STR+LUK", "DEX+INT", "DEX+LUK", "INT+LUK",
		"DEF", "ALL", "JMP"
	};
	// �ߺ� �ɼ� üũ
	std::map<std::string, int> m_Duplicate;

	// ��ȭ��ġ, �ش� ������
	std::map<int, std::discrete_distribution<>> m_Starforce_Table;
	// ��ȯ��, �ش� ������
	std::map<int, std::discrete_distribution<>> m_Rebirth_Table;
	// �߿ɿ� ���� ������ (����, ����/����)
	std::map<int, std::pair<int, int>> m_Additional_Table;
	// ���� �����
	std::queue<Weapon*> m_Weapon_List;
	// ��ȭ�� ���� ����
	std::vector<int> m_After_Weapon;
	// ��ȯ�� ��, �߿��� �� ����
	Weapon m_Weapon;
	std::vector<double> m_CSV_Data;
	int m_ColumCount = 0;
	bool m_Is_Once = false;

private:
	// double ������ csv���� �б�
	void ReadDoubleCSV(const std::string& _Path);

public:
	// ��Ÿ���� Ȯ�� csv ����
	void SetStarForce(const std::string& _Path);
	// ��ȯ�� Ȯ�� csv ����
	void SetRebirthFireOption(const std::string& _Path);
	// �� �߿� Ȯ�� csv ����
	void SetAdditionalOption(const std::string& _Path);
	// ���� ����
	void MakeWeapon(int _Count);
	// ��Ÿ���� ��ȭ!
	void ReinforceStarforce();
	// ��ȯ�� / ��ȯ�� �õ�!
	void ReinforceRebirth();

private:
	// ���� �ɼ� ���
	void PrintWeapon();
	// ���� �ɼ� �ޱ�
	void SetOption(std::mt19937& _gen);
};

