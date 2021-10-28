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
	// Fmod 시스템
	FMOD::System* Sound_System = nullptr;
	// 총 사운드에 대한 마스터그룹.
	FMOD::ChannelGroup* Master_Group = nullptr;
	// Sound 그룹의 종류 BGM/SFX 두개.
	FMOD::ChannelGroup* BGM_Group = nullptr;
	FMOD::ChannelGroup* SFX_Group = nullptr;
	// 외부에서 로드한 사운드 리소스들을 관리할 리스트.
	std::map<ATL::CString, FMOD::Sound*> Sound_Resource;
	// 채널을 관리할 채널 리스트
	std::vector<FMOD::Channel*> Channel_Vec;
	// BGM path
	ATL::CString BGM_Path = "NonSet";
	// SFX path
	ATL::CString SFX_Path = "NonSet";
	// 사운드 설정
	Sound_VFP Master_VFP;
	Sound_VFP SFX_VFP;
	Sound_VFP BGM_VFP;
	// 볼륨의 Up Down을 호출했을 때 가중치
	float Volume_Factor = 0.1f;
	float Frequency_Factor = 100.f;
	float Pitch_Factor = 0.1f;

private:
	// Fmod Default Setting
	void Initialize();

	// Sound_Category에 따라 ChannelGroup 반환.
	FMOD::ChannelGroup* GetChannelGroup(Sound_Category _Sound_Category);
	Sound_VFP* GetVFP(Sound_Category _Sound_Category);
	ATL::CString* GetFolderPath(Sound_Category _Sound_Category);
	FMOD::Channel* GetChannel();

public:
	DHSound();
	~DHSound();

	// Fmod Update
	void Update();
	// 경로설정
	void SetSoundFolderPath(Sound_Category _Sound_Category, ATL::CString _Folder_Path);
	// 해당 파일을 불러와서 세팅.
	void LoadSound(Sound_Category _Sound_Category, ATL::CString _Sound_Key, ATL::CString _File_Path, bool _Loop);
	// 소리 크기 설정
	void SetVolumeFactor(float _Set_Factor);
	void VolumeSet(Sound_Category _Sound_Category, float _Set_Volume);
	void VolumeUp(Sound_Category _Sound_Category);
	void VolumeDown(Sound_Category _Sound_Category);
	// 소리 Frequency 설정 (역재생도 가능)
	void SetFrequencyFactor(float _Set_Factor);
	void FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency);
	void FrequencyUp(Sound_Category _Sound_Category);
	void FrequencyDown(Sound_Category _Sound_Category);
	// 소리 Pitch 설정 (재생속도 역재생 불가)
	void SetPitchFactor(float _Set_Factor);
	void PitchSet(Sound_Category _Sound_Category, float _Set_Pitch);
	void PitchUp(Sound_Category _Sound_Category);
	void PitchDown(Sound_Category _Sound_Category);
	// 사운드 재생 관련
	void PlaySound(Sound_Category _Sound_Category, ATL::CString _Sound_Key);
	void StopSound(Sound_Category _Sound_Category);
	void PauseSound(Sound_Category _Sound_Category, bool _Play);
	
/// 3D 사운드
public:
	void Set3DListener(DirectX::XMMATRIX* _View_Matrix, DirectX::XMFLOAT3 _Velocity);
	void Set3DSoundObject(Sound_Category _Sound_Category, ATL::CString _Sound_Key, DirectX::XMFLOAT3 _Position, DirectX::XMFLOAT3 _Velocity);

private:
	FMOD_VECTOR ConvertFmodVector(DirectX::XMFLOAT3 _Vector3);
};

