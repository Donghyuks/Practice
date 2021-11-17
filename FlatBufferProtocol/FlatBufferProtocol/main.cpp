#include "flatbuffers/flatbuffers.h"
#include "ShareProtocol/FlatBuffers_generated.h"
#include <vector>
#include <iostream>


int main()
{
	/// 플랫버퍼의 빌더 생성.
	flatbuffers::FlatBufferBuilder Flat_Builder;
	
	/// 무기의 이름
	auto Weapon1 = Flat_Builder.CreateString("엑스칼리버");
	short Weapon1_Damage = 2;

	/// 무기하나 만듬.
	auto 엑스칼리버 = MyGame::Sample::CreateWeapon(Flat_Builder, Weapon1, Weapon1_Damage);

	/// std::vector에 faltbuffer 의 벡터를 만듬
	std::vector<flatbuffers::Offset<MyGame::Sample::Weapon>> my_Weapon_List;
	my_Weapon_List.push_back(엑스칼리버);

	auto Weapons = Flat_Builder.CreateVector(my_Weapon_List);

	/// 몬스터에 필요한 좌표, 이름, 인벤토리 등을 만들어 줌.
	auto Monster_Position = MyGame::Sample::Vec3(1.f, 2.f, 3.f);

	auto Monster_Name = Flat_Builder.CreateString("존나쎈몬스터");

	/// std::vector 가 아닌 배열로써 생성..
	unsigned char Inventory_Data[] = { 0,1,2,3,4,5,6,7,8,9 };
	auto Inventory = Flat_Builder.CreateVector(Inventory_Data, 10);

	/// 모든 데이터가 설정된 후 몬스터를 생성한다.
	auto Final_Monster = MyGame::Sample::CreateMonster(Flat_Builder, &Monster_Position, 150, 80,
		Monster_Name, Inventory, MyGame::Sample::Color_Red, Weapons, MyGame::Sample::Equipment_Weapon,
		엑스칼리버.Union());

	/// 데이터 빌드.
	Flat_Builder.Finish(Final_Monster);

	/*
		데이터를 받아오면 해당 데이터를 빌드한다.	
	*/

	/// 데이터 받기
	auto Recv_Monster_Data = MyGame::Sample::GetMonster(Flat_Builder.GetBufferPointer());

	std::cout << "Monster HP : " << Recv_Monster_Data->hp() << std::endl;
	std::cout << "Monster Mana : " << Recv_Monster_Data->mana() << std::endl;
	std::cout << "Monster Name : " << Recv_Monster_Data->name()->str() << std::endl;

	return 0;
}