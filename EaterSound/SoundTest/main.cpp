#include "EaterSound.h"
#include "KeyIO.h"

int main()
{
	EaterSound m_Sound;

	// ���ϰ�� ����
	m_Sound.SetSoundFolderPath(Sound_Category::BGM, "../TestResource/BGM/");
	m_Sound.SetSoundFolderPath(Sound_Category::SFX, "../TestResource/SFX/");

	// ���� �ε�
	m_Sound.LoadSound(Sound_Category::BGM, "Ending", "BGM_Ending.mp3", true);
	m_Sound.LoadSound(Sound_Category::SFX, "Siren", "SFX_Siren.wav", false);
	m_Sound.LoadSound(Sound_Category::SFX, "GameOver", "SFX_GameOver.wav", false);

	// BGM ���
	m_Sound.PlaySoundW(Sound_Category::BGM, "Ending");
	//m_Sound.PlaySoundW(Sound_Category::SFX, "Siren");
	//m_Sound.PlaySoundW(Sound_Category::SFX, "GameOver");

	while (true)
	{
		m_Sound.Update();

		// �Ҹ���� ����
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F1))
		{
			m_Sound.PauseSound(Sound_Category::BGM, true);
		}
		// �ٽ� ���
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F2))
		{
			m_Sound.PauseSound(Sound_Category::BGM, false);
		}
		// ���� ���̱�
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F3))
		{
			m_Sound.VolumeDown(Sound_Category::BGM);
		}
		// ���� ���̱�
		if (KeyIO::GetInstance()->Is_KeyDown(VK_F4))
		{
			m_Sound.VolumeUp(Sound_Category::BGM);
		}
		// �������� ���̱�
		if (KeyIO::GetInstance()->Is_KeyDown('W'))
		{
			m_Sound.FrequencyUp(Sound_Category::BGM);
		}
		// �������� ���߱�
		if (KeyIO::GetInstance()->Is_KeyDown('Q'))
		{
			m_Sound.FrequencyDown(Sound_Category::BGM);
		}
		// ��ġ ���̱�
		if (KeyIO::GetInstance()->Is_KeyDown('D'))
		{
			m_Sound.PitchUp(Sound_Category::BGM);
		}
		// ��ġ ���߱�
		if (KeyIO::GetInstance()->Is_KeyDown('A'))
		{
			m_Sound.PitchDown(Sound_Category::BGM);
		}
	}

}