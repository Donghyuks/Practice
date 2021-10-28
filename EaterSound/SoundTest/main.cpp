#include "EaterSound.h"
#include "KeyIO.h"

int main()
{
	EaterSound m_Sound;

	// 파일경로 세팅
	m_Sound.SetSoundFolderPath(Sound_Category::BGM, "../TestResource/BGM/");
	m_Sound.SetSoundFolderPath(Sound_Category::SFX, "../TestResource/SFX/");

	// 사운드 로드
	m_Sound.LoadSound(Sound_Category::BGM, "Ending", "BGM_Ending.mp3", true);
	m_Sound.LoadSound(Sound_Category::SFX, "Siren", "SFX_Siren.wav", false);
	m_Sound.LoadSound(Sound_Category::SFX, "GameOver", "SFX_GameOver.wav", false);

	// BGM 재생
	m_Sound.PlaySoundW(Sound_Category::BGM, "Ending");
	//m_Sound.PlaySoundW(Sound_Category::SFX, "Siren");
	//m_Sound.PlaySoundW(Sound_Category::SFX, "GameOver");

	while (true)
	{
		m_Sound.Update();

		// 소리재생 멈춤
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F1))
		{
			m_Sound.PauseSound(Sound_Category::BGM, true);
		}
		// 다시 재생
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F2))
		{
			m_Sound.PauseSound(Sound_Category::BGM, false);
		}
		// 볼륨 줄이기
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F3))
		{
			m_Sound.VolumeDown(Sound_Category::BGM);
		}
		// 볼륨 높이기
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F4))
		{
			m_Sound.VolumeUp(Sound_Category::BGM);
		}
		// 프리퀀시 높이기
		if (KeyIO::GetInstance()->Is_KeyDown('W'))
		{
			m_Sound.FrequencyUp(Sound_Category::BGM);
		}
		// 프리퀀시 낮추기
		if (KeyIO::GetInstance()->Is_KeyDown('Q'))
		{
			m_Sound.FrequencyDown(Sound_Category::BGM);
		}
		// 피치 높이기
		if (KeyIO::GetInstance()->Is_KeyDown('D'))
		{
			m_Sound.PitchUp(Sound_Category::BGM);
		}
		// 피치 낮추기
		if (KeyIO::GetInstance()->Is_KeyDown('A'))
		{
			m_Sound.PitchDown(Sound_Category::BGM);
		}
	}

}