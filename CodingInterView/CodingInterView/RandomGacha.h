#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <random>

struct Weapon
{
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
	// ��ȭ��ġ, �ش� ������
	std::map<int, std::discrete_distribution<>> m_Starforce_Table;
	// ���� �����
	std::queue<Weapon*> m_Weapon_List;
	// ��ȭ�� ���� ����
	std::vector<int> m_After_Weapon;

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
	// �� �߿�(1�� ������ Ȯ��) �߰�
	void AddFixed_AdditionalOption(const std::string& _Option_Name);
	// ���� ����
	void MakeWeapon(int _Count);
	// ��Ÿ���� ��ȭ!
	void ReinforceStarforce();
};

