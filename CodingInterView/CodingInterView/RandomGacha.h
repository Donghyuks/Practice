#pragma once

#include <string>
#include <vector>

class RandomGacha
{
private:
	std::vector<double> m_CSV_Data;
	int m_ColumCount = 0;

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

};

