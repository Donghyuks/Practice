
#include "GameManager.h"
#include "DHKeyIO.h"
#include "DHTimer.h"
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "WorldData_generated.h"
#include "PortIPDefine.h"

HRESULT GameManager::Initialize(HINSTANCE hInstance)
{
	m_KeyIO = new DHKeyIO();
	m_Timer = new DHTimer();
	m_Timer->SetFrame(60);

	_Send_Packet = new C2S_Packet();

	/// 임시 플레이어 데이터.
	Player* _Player01 = new Player();
	m_Player.insert({ EATER_PLAYER_01 , _Player01 });

	Player* _Player02 = new Player();
	m_Player.insert({ EATER_CHARACTER_02 , _Player02 });

	/// Win32 관련
	// 윈도 클래스
	LPCWSTR WinName = L"Eater Client Demo";
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = GameManager::WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = WinName;

	// 윈도 클래스 등록
	RegisterClass(&wndclass);

	// 윈도 생성
	g_hwnd = CreateWindowW(WinName, WinName, WS_OVERLAPPEDWINDOW,
		0, 0, WORLD_SCREEN_X, WORLD_SCREEN_Y, nullptr, nullptr, hInstance, nullptr);

	if (!g_hwnd) return S_FALSE;

	// 서버 연결..
	m_Network = new DHNetWorkAPI();
	m_Network->Initialize(DHNetWork_Name::DHNet);
	while (!m_Network->Connect(GAME_SERVER_PORT, LOCAL_CONNECT_IP)) {};

	// 그래픽스 엔진 초기화
	MYD2D->Initialize(g_hwnd);

	// 생성된 윈도를 화면에 표시
	ShowWindow(g_hwnd, SW_SHOWNORMAL);
	UpdateWindow(g_hwnd);

	Object* Monster = new Object(MYD2D, WORLD_SCREEN_X / 2, WORLD_SCREEN_Y / 2, 0);

	Obj_List.push_back(Monster);
	MYD2D->SetGridMod(true, 100.0f, 100.0f, D2D1::ColorF(0.5, 0.5, 0.5));

	string Pre_Path = "../image/";
	string File_Name = "Reload_11.bmp";
	Monster->AddAnimation(11, 0.5, Pre_Path + File_Name);
	File_Name = "atk_1.png";
	Monster->AddAnimation(7, 0.5, Pre_Path + File_Name);
	File_Name = "Attack_R.bmp";
	Monster->AddAnimation(8, 0.5, Pre_Path + File_Name);
	File_Name = "slime_8.bmp";
	Monster->AddAnimation(8, 0.5, Pre_Path + File_Name);
	File_Name = "battle_player_attack.bmp";
	Monster->AddAnimation(16, 1, Pre_Path + File_Name);
	File_Name = "Chicken_move_G.bmp";
	Monster->AddAnimation(7, 0.5, Pre_Path + File_Name);
	File_Name = "enemy_grasshopper.bmp";
	Monster->AddAnimation(4, 0.5, Pre_Path + File_Name);
	File_Name = "kkangchong.bmp";
	Monster->AddAnimation(5, 0.5, Pre_Path + File_Name);
	File_Name = "run_human.bmp";
	Monster->AddAnimation(5, 0.5, Pre_Path + File_Name);
	File_Name = "Side_Drill_Left_320x60.bmp";
	Monster->AddAnimation(4, 0.5, Pre_Path + File_Name);

	//Obj_List.at(0)->GetTransform()->SetTRSrotation(1);
	//Obj_List.at(0)->GetTransform()->SetTRSscale(0.0005, 0.0005);

	// 로딩완료
	_Send_Packet->Packet_Type = C2S_LOADING_COMPLETE_REQ;
	_Send_Packet->Packet_Size = 0;
	m_Network->Send(_Send_Packet);

	return S_OK;
}

void GameManager::Loop()
{
	while (true)
	{
		m_Timer->Update();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			DispatchMessage(&msg);
		}

		if (m_Timer->Ready_Frame())
		{
			GameLoop();
		}

		Sleep(0);
	}
}

void GameManager::GameLoop()
{
	static Transform trs;
	static int GetFrame = 0;
	static int Current_Ani = 0;

	std::vector<Network_Message> Msg_Vec;
	S2C_Packet* Recv_Packet = nullptr;

	if (m_Network->Recv(Msg_Vec))
	{
		for (auto& S2C_Msg : Msg_Vec)
		{
			Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);

			// 처음에 내가 몇번째 유저인지 알려준다.
			if (Recv_Packet->Packet_Type == S2C_LOADING_COMPLETE_RES)
			{
				// 데이터 캐스팅.
				unsigned short* Recv_Player_Index = (unsigned short*)Recv_Packet->Packet_Buffer;

				m_Player_Number = *Recv_Player_Index;
				
			}

			if (Recv_Packet->Packet_Type == S2C_START_GAME)
			{
				// 데이터 캐스팅.
				//const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
				//const Eater::GameData::ClientStartInfo* _Recv_Data = flatbuffers::GetRoot<Eater::GameData::ClientStartInfo>(Recv_Data_Ptr);

				//m_Player_Number = _Recv_Data->playerindex();

			}
		}
	}

	MYD2D->StartRender();
	MYD2D->thDrawText(10, 10, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Current FPS : %.6f FPS", m_Timer->GetFrameTime());

	MYD2D->thDrawEllipse(200, 200, 10);
	MYD2D->thDrawEllipse(400, 200, 10);

	//GetFrame++;

	//if (GetFrame > 120)
	//{
	//	GetFrame = 0;
	//	if (Current_Ani > 9) Current_Ani = 0;
	//	Obj_List.at(0)->SetAnimationNum(Current_Ani++);
	//}

	//for (auto k : Obj_List)
	//{
	//	k->Update();
	//}

	//for (auto k : Obj_List)
	//{
	//	k->Draw();
	//}

	MYD2D->EndRender();

}

void GameManager::Finalize()
{
	MYD2D->DiscardGraphicsResources();
}

// 메시지 핸들러 (윈도 콜백)
LRESULT CALLBACK GameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
