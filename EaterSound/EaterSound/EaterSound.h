#pragma once

#include "DHSoundDefine.h"

namespace DirectX
{
	struct XMMATRIX;
	struct XMFLOAT3;
}
class DHSound;

class EATERSOUND_DLL EaterSound
{
public:
	EaterSound();
	~EaterSound();

	// Fmod Update
	void Update();
	// ��μ���
	void SetSoundFolderPath(Sound_Category _Sound_Category, ATL::CString _Folder_Path);
	// �ش� ������ �ҷ��ͼ� ����.
	void LoadSound(Sound_Category _Sound_Category, ATL::CString _Sound_Key, ATL::CString _File_Path, bool _Loop);
	// �Ҹ� ũ�� ����
	void SetVolumeFactor(float _Set_Factor);
	void VolumeSet(Sound_Category _Sound_Category, float _Set_Volume);
	void VolumeUp(Sound_Category _Sound_Category);
	void VolumeDown(Sound_Category _Sound_Category);
	// �Ҹ� Frequency ����
	void SetFrequencyFactor(float _Set_Factor);
	void FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency);
	void FrequencyUp(Sound_Category _Sound_Category);
	void FrequencyDown(Sound_Category _Sound_Category);
	// �Ҹ� Pitch ����
	void SetPitchFactor(float _Set_Factor);
	void PitchSet(Sound_Category _Sound_Category, float _Set_Pitch);
	void PitchUp(Sound_Category _Sound_Category);
	void PitchDown(Sound_Category _Sound_Category);
	// ���� ��� ����
	void PlaySound(Sound_Category _Sound_Category, ATL::CString _Sound_Key);
	void StopSound(Sound_Category _Sound_Category);
	void PauseSound(Sound_Category _Sound_Category, bool _Play);

/// 3D ����
public:
	void Set3DListener(DirectX::XMMATRIX* _View_Matrix, DirectX::XMFLOAT3 _Velocity);
	void Set3DSoundObject(DirectX::XMMATRIX* _World_Trans, DirectX::XMFLOAT3 _Velocity);

private:
	DHSound* m_Sound;
};