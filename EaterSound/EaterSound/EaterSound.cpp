#include "EaterSound.h"
#include "DHSound.h"
#include <DirectXMath.h>

EaterSound::EaterSound()
{
	m_Sound = new DHSound;
}

EaterSound::~EaterSound()
{
	delete m_Sound;
}

void EaterSound::Update()
{
	m_Sound->Update();
}

void EaterSound::SetSoundFolderPath(Sound_Category _Sound_Category, ATL::CString _Folder_Path)
{
	m_Sound->SetSoundFolderPath(_Sound_Category, _Folder_Path);
}

void EaterSound::LoadSound(Sound_Category _Sound_Category, ATL::CString _Sound_Key, ATL::CString _File_Path, bool _Loop)
{
	m_Sound->LoadSound(_Sound_Category, _Sound_Key, _File_Path, _Loop);
}

void EaterSound::SetVolumeFactor(float _Set_Factor)
{
	m_Sound->SetVolumeFactor(_Set_Factor);
}

void EaterSound::VolumeSet(Sound_Category _Sound_Category, float _Set_Volume)
{
	m_Sound->VolumeSet(_Sound_Category, _Set_Volume);
}

void EaterSound::VolumeUp(Sound_Category _Sound_Category)
{
	m_Sound->VolumeUp(_Sound_Category);
}

void EaterSound::VolumeDown(Sound_Category _Sound_Category)
{
	m_Sound->VolumeDown(_Sound_Category);
}

void EaterSound::SetFrequencyFactor(float _Set_Factor)
{
	m_Sound->SetFrequencyFactor(_Set_Factor);
}

void EaterSound::FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency)
{
	m_Sound->FrequencySet(_Sound_Category, _Set_Frequency);
}

void EaterSound::FrequencyUp(Sound_Category _Sound_Category)
{
	m_Sound->FrequencyUp(_Sound_Category);
}

void EaterSound::FrequencyDown(Sound_Category _Sound_Category)
{
	m_Sound->FrequencyDown(_Sound_Category);
}

void EaterSound::SetPitchFactor(float _Set_Factor)
{
	m_Sound->SetPitchFactor(_Set_Factor);
}

void EaterSound::PitchSet(Sound_Category _Sound_Category, float _Set_Pitch)
{
	m_Sound->PitchSet(_Sound_Category, _Set_Pitch);
}

void EaterSound::PitchUp(Sound_Category _Sound_Category)
{
	m_Sound->PitchUp(_Sound_Category);
}

void EaterSound::PitchDown(Sound_Category _Sound_Category)
{
	m_Sound->PitchDown(_Sound_Category);
}

void EaterSound::PlaySound(Sound_Category _Sound_Category, ATL::CString _Sound_Key)
{
	m_Sound->PlaySound(_Sound_Category, _Sound_Key);
}

void EaterSound::StopSound(Sound_Category _Sound_Category)
{
	m_Sound->StopSound(_Sound_Category);
}

void EaterSound::PauseSound(Sound_Category _Sound_Category, bool _Play)
{
	m_Sound->PauseSound(_Sound_Category, _Play);
}

void EaterSound::Set3DListener(DirectX::XMMATRIX* _View_Matrix, DirectX::XMFLOAT3 _Velocity)
{
	// ī�޶��� ��ġ�� �� ����� ����Ŀ��� ���� �� �ִ�.
	DirectX::XMFLOAT4X4 Inverse_View_Matrix;
	DirectX::XMVECTOR det;
	XMStoreFloat4x4(&Inverse_View_Matrix, XMMatrixInverse(&det, *_View_Matrix));

	FMOD_3D_ATTRIBUTES listener;
	// �������� ��ġ�� ���� ����, ���� ���͸� ����
	listener.position = VecToFMOD(invView.GetTranslation());
	// �� ����� ����Ŀ��� �� ��° ���� ���� ����
	listener.forward = VecToFMOD(invView.GetZAxis());
	// �� ����� ����Ŀ��� �� ��° ���� ���� ����
	listener.up = VecToFMOD(invView.GetYAxis());
	// �������� �ӵ� ����. ���÷� ȿ���� �����ϱ� ���� �ʿ�
	listener.velocity = VecToFMOD(velocity);
	// FMOD�� ������ (0 = �����ʴ� �ϳ�)
	mSystem->setListenerAttributes(0, &listener);
}

void EaterSound::Set3DSoundObject(DirectX::XMMATRIX* _World_Trans, DirectX::XMFLOAT3 _Velocity)
{

}
