#pragma once

#include "DHSoundDefine.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include <map>
#include <vector>
#include <DirectXMath.h>

class DHSound
{
private:
	// Fmod �ý���
	FMOD::System* Sound_System = nullptr;
	// �� ���忡 ���� �����ͱ׷�.
	FMOD::ChannelGroup* Master_Group = nullptr;
	// Sound �׷��� ���� BGM/SFX �ΰ�.
	FMOD::ChannelGroup* BGM_Group = nullptr;
	FMOD::ChannelGroup* SFX_Group = nullptr;
	// �ܺο��� �ε��� ���� ���ҽ����� ������ ����Ʈ.
	std::map<ATL::CString, FMOD::Sound*> Sound_Resource;
	// ä���� ������ ä�� ����Ʈ
	std::vector<FMOD::Channel*> Channel_Vec;
	// BGM path
	ATL::CString BGM_Path = "NonSet";
	// SFX path
	ATL::CString SFX_Path = "NonSet";
	// ���� ����
	Sound_VFP Master_VFP;
	Sound_VFP SFX_VFP;
	Sound_VFP BGM_VFP;
	// ������ Up Down�� ȣ������ �� ����ġ
	float Volume_Factor = 0.1f;
	float Frequency_Factor = 100.f;
	float Pitch_Factor = 0.1f;

private:
	// Fmod Default Setting
	void Initialize();

	// Sound_Category�� ���� ChannelGroup ��ȯ.
	FMOD::ChannelGroup* GetChannelGroup(Sound_Category _Sound_Category);
	Sound_VFP* GetVFP(Sound_Category _Sound_Category);
	ATL::CString* GetFolderPath(Sound_Category _Sound_Category);
	FMOD::Channel* GetChannel();

public:
	DHSound();
	~DHSound();

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
	// �Ҹ� Frequency ���� (������� ����)
	void SetFrequencyFactor(float _Set_Factor);
	void FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency);
	void FrequencyUp(Sound_Category _Sound_Category);
	void FrequencyDown(Sound_Category _Sound_Category);
	// �Ҹ� Pitch ���� (����ӵ� ����� �Ұ�)
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
	void Set3DSoundObject(Sound_Category _Sound_Category, ATL::CString _Sound_Key, DirectX::XMFLOAT3 _Position, DirectX::XMFLOAT3 _Velocity);

private:
	FMOD_VECTOR ConvertFmodVector(DirectX::XMFLOAT3 _Vector3);
};

