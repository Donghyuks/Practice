#include "LobbyManager.h"

LobbyManager* LobbyManager::m_Instance = nullptr;

LobbyManager* LobbyManager::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new LobbyManager();
	}

	return m_Instance;
}

LobbyManager::LobbyManager()
{

}

LobbyManager::~LobbyManager()
{

}

void LobbyManager::Initialize(std::string _IP, int PORT)
{

}
