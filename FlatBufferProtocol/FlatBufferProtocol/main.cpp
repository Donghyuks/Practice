#include "flatbuffers/flatbuffers.h"
#include "ShareProtocol/FlatBuffers_generated.h"
#include <vector>
#include <iostream>


int main()
{
	/// �÷������� ���� ����.
	flatbuffers::FlatBufferBuilder Flat_Builder;
	
	/// ������ �̸�
	auto Weapon1 = Flat_Builder.CreateString("����Į����");
	short Weapon1_Damage = 2;

	/// �����ϳ� ����.
	auto ����Į���� = MyGame::Sample::CreateWeapon(Flat_Builder, Weapon1, Weapon1_Damage);

	/// std::vector�� faltbuffer �� ���͸� ����
	std::vector<flatbuffers::Offset<MyGame::Sample::Weapon>> my_Weapon_List;
	my_Weapon_List.push_back(����Į����);

	auto Weapons = Flat_Builder.CreateVector(my_Weapon_List);

	/// ���Ϳ� �ʿ��� ��ǥ, �̸�, �κ��丮 ���� ����� ��.
	auto Monster_Position = MyGame::Sample::Vec3(1.f, 2.f, 3.f);

	auto Monster_Name = Flat_Builder.CreateString("���������");

	/// std::vector �� �ƴ� �迭�ν� ����..
	unsigned char Inventory_Data[] = { 0,1,2,3,4,5,6,7,8,9 };
	auto Inventory = Flat_Builder.CreateVector(Inventory_Data, 10);

	/// ��� �����Ͱ� ������ �� ���͸� �����Ѵ�.
	auto Final_Monster = MyGame::Sample::CreateMonster(Flat_Builder, &Monster_Position, 150, 80,
		Monster_Name, Inventory, MyGame::Sample::Color_Red, Weapons, MyGame::Sample::Equipment_Weapon,
		����Į����.Union());

	/// ������ ����.
	Flat_Builder.Finish(Final_Monster);

	/*
		�����͸� �޾ƿ��� �ش� �����͸� �����Ѵ�.	
	*/

	/// ������ �ޱ�
	auto Recv_Monster_Data = MyGame::Sample::GetMonster(Flat_Builder.GetBufferPointer());

	std::cout << "Monster HP : " << Recv_Monster_Data->hp() << std::endl;
	std::cout << "Monster Mana : " << Recv_Monster_Data->mana() << std::endl;
	std::cout << "Monster Name : " << Recv_Monster_Data->name()->str() << std::endl;

	return 0;
}