
#include <windows.h>
#include "GameManager.h"

// ���α׷��� ���� ������(Entry Point). ��Ƽ����Ʈ ������Ʈ
int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ���� ���� Ŭ����
	// ���ο��� ���� ������ �޽��� �ݹ��� ó���Ѵ�.
	// ���� �׷��Ƚ� ���� ���� �����Ѵ�.

	GameManager* GM = new GameManager();
	GM->Initialize(hInstance);
	GM->Loop();
	GM->Finalize();

	return 0; // (int)msg.wParam;
}