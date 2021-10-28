#pragma once

#include "DHSoundDefine.h"

class DHSound;

class EATERSOUND_DLL EaterSound
{
public:
	EaterSound();
	~EaterSound();

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
	// 소리 Frequency 설정
	void SetFrequencyFactor(float _Set_Factor);
	void FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency);
	void FrequencyUp(Sound_Category _Sound_Category);
	void FrequencyDown(Sound_Category _Sound_Category);
	// 소리 Pitch 설정
	void SetPitchFactor(float _Set_Factor);
	void PitchSet(Sound_Category _Sound_Category, float _Set_Pitch);
	void PitchUp(Sound_Category _Sound_Category);
	void PitchDown(Sound_Category _Sound_Category);
	// 사운드 재생 관련
	void PlaySound(Sound_Category _Sound_Category, ATL::CString _Sound_Key);
	void StopSound(Sound_Category _Sound_Category);
	void PauseSound(Sound_Category _Sound_Category, bool _Play);

private:
	DHSound* m_Sound;
};