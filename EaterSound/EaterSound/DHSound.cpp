#include "DHSound.h"
#include <string>

DHSound::DHSound()
{
	Initialize();
}

DHSound::~DHSound()
{
	Sound_System->close();
}

void DHSound::Initialize()
{
	// 시스템 생성 및 기본 세팅 ( CHANNEL_COUNT 만큼 채널 생성 )
	FMOD::System_Create(&Sound_System);
	Sound_System->init(CHANNEL_COUNT, FMOD_INIT_NORMAL, nullptr);
	// 채널 그룹 미리 생성
	Sound_System->createChannelGroup("SFX", &SFX_Group);
	Sound_System->createChannelGroup("BGM", &BGM_Group);
	Sound_System->getMasterChannelGroup(&Master_Group);
	// 마스터그룹에 SFX,BGM 그룹 배당.
	Master_Group->addGroup(SFX_Group);
	Master_Group->addGroup(BGM_Group);

	Channel_Vec.resize(CHANNEL_COUNT);
}

FMOD::ChannelGroup* DHSound::GetChannelGroup(Sound_Category _Sound_Category)
{
	switch (_Sound_Category)
	{
	case Sound_Category::BGM:
		return BGM_Group;
	case Sound_Category::SFX:
		return SFX_Group;
	case Sound_Category::Master:
		return Master_Group;
	}
}

Sound_VFP* DHSound::GetVFP(Sound_Category _Sound_Category)
{
	switch (_Sound_Category)
	{
	case Sound_Category::BGM:
		return &BGM_VFP;
	case Sound_Category::SFX:
		return &SFX_VFP;
	case Sound_Category::Master:
		return &Master_VFP;
	}
}

ATL::CString* DHSound::GetFolderPath(Sound_Category _Sound_Category)
{
	switch (_Sound_Category)
	{
	case Sound_Category::BGM:
		return &BGM_Path;
	case Sound_Category::SFX:
		return &SFX_Path;
	case Sound_Category::Master:
	{
		printf("[DHSound Wrong Category Error] Sound_Category의 값이 잘못 되었습니다.");
		return nullptr;
	}
	}
}

FMOD::Channel* DHSound::GetChannel()
{
	bool _Is_Play = false;

	for (auto _Ready_Channel : Channel_Vec)
	{
		_Ready_Channel->isPlaying(&_Is_Play);
		if (_Is_Play == false)
		{
			return _Ready_Channel;
		}
	}

	printf("[DHSound Not Enough Channel] 채널이 모두 사용중입니다.");
	return nullptr;
}

void DHSound::Update()
{
	Sound_System->update();
}

void DHSound::SetSoundFolderPath(Sound_Category _Sound_Category, ATL::CString _Folder_Path)
{
	// 해당하는 path 를 들고온다.
	ATL::CString* _Path = GetFolderPath(_Sound_Category);

	if (_Path == nullptr)
	{
		return;
	}

	// 입력받은 경로 설정.
	*_Path = _Folder_Path;
}

void DHSound::LoadSound(Sound_Category _Sound_Category, ATL::CString _Sound_Key, ATL::CString _File_Path, bool _Loop)
{
	// 해당하는 path 를 들고온다.
	ATL::CString* _Path = GetFolderPath(_Sound_Category);

	if (_Path == nullptr || *_Path == "NonSet")
	{
		printf("[DHSound LoadSound Error] Sound를 읽어오지 못했습니다. 폴더 경로를 먼저 설정하거나 올바른 Category를 지정하세요.");
		return;
	}

	// 해당하는 파일에 총 path를 완성시킨다.
	std::string _Totla_Path = std::string(CT2CA(*_Path + _File_Path));

	FMOD::Sound* _New_Sound = nullptr;

	// 해당하는 사운드 생성
	if (_Loop)	{ Sound_System->createSound(_Totla_Path.c_str(), FMOD_LOOP_NORMAL, nullptr, &_New_Sound); }
	else		{ Sound_System->createSound(_Totla_Path.c_str(), FMOD_DEFAULT, nullptr, &_New_Sound); }

	// 사운드 리스트에 추가.
	Sound_Resource.insert({ _Sound_Key, _New_Sound });
}

void DHSound::SetVolumeFactor(float _Set_Factor)
{
	Volume_Factor = _Set_Factor;
}

void DHSound::VolumeSet(Sound_Category _Sound_Category, float _Set_Volume)
{
	if (_Set_Volume < 0) { _Set_Volume = 0; }
	if (_Set_Volume > 1) { _Set_Volume = 1.f; }

	// 해당 채널그룹과 볼륨값을 가져와서 세팅해줌.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = GetVFP(_Sound_Category);

	_Vol->Volume = _Set_Volume;
	_CG->setVolume(_Set_Volume);
}

void DHSound::VolumeUp(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 볼륨값을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = GetVFP(_Sound_Category);

	// 현재 지정된 볼륨의 가중치에 따라 볼륨을 증가시켜 적용한다.
	_Vol->Volume += Volume_Factor;

	if (_Vol->Volume > 1) { _Vol->Volume = 1.f; }

	_CG->setVolume(_Vol->Volume);
}

void DHSound::VolumeDown(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 볼륨값을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Vol = GetVFP(_Sound_Category);

	// 현재 지정된 볼륨의 가중치에 따라 볼륨을 감소시켜 적용한다.
	_Vol->Volume -= Volume_Factor;

	if (_Vol->Volume > 1) { _Vol->Volume = 1.f; }

	_CG->setVolume(_Vol->Volume);
}

void DHSound::SetFrequencyFactor(float _Set_Factor)
{
	Frequency_Factor = _Set_Factor;
}

void DHSound::FrequencySet(Sound_Category _Sound_Category, float _Set_Frequency)
{
	// 해당 채널그룹과 프리퀀시값을 가져와서 세팅해줌.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = GetVFP(_Sound_Category);

	_Fre->FreQuency = _Set_Frequency;
	_CG->setVolume(_Set_Frequency);
}

void DHSound::FrequencyUp(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 프리퀀시값을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = GetVFP(_Sound_Category);

	// 현재 지정된 프리퀀시의 가중치에 따라 증가시켜 적용한다.
	_Fre->FreQuency += Frequency_Factor;

	// 지정된 채널의 모든 프리퀀시를 높임.
	int Channel_Nums = 0;
	_CG->getNumChannels(&Channel_Nums);

	for (int i = 0; i < Channel_Nums; i++)
	{
		FMOD::Channel* _Channel = nullptr;
		_CG->getChannel(i, &_Channel);
		_Channel->setFrequency(_Fre->FreQuency);
	}
}

void DHSound::FrequencyDown(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 프리퀀시값을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Fre = GetVFP(_Sound_Category);

	// 현재 지정된 프리퀀시의 가중치에 따라 증가시켜 적용한다.
	_Fre->FreQuency -= Frequency_Factor;

	// 지정된 채널의 모든 프리퀀시를 낮춤.
	int Channel_Nums = 0;
	_CG->getNumChannels(&Channel_Nums);

	for (int i = 0; i < Channel_Nums; i++)
	{
		FMOD::Channel* _Channel = nullptr;
		_CG->getChannel(i, &_Channel);
		_Channel->setFrequency(_Fre->FreQuency);
	}
}

void DHSound::SetPitchFactor(float _Set_Factor)
{
	Pitch_Factor = _Set_Factor;
}

void DHSound::PitchSet(Sound_Category _Sound_Category, float _Set_Pitch)
{
	if (_Set_Pitch < 0) { _Set_Pitch = 0; }
	// 해당 채널그룹과 피치값을 가져와서 세팅해줌.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = GetVFP(_Sound_Category);

	_Pitch->Pitch = _Set_Pitch;
	_CG->setVolume(_Set_Pitch);
}

void DHSound::PitchUp(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 피치값을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = GetVFP(_Sound_Category);

	// 현재 지정된 피치의 가중치에 따라 증가시켜 적용한다.
	_Pitch->Pitch += Pitch_Factor;

	// 지정된 채널의 모든 피치를 높임.
	int Channel_Nums = 0;
	_CG->getNumChannels(&Channel_Nums);


	for (int i = 0; i < Channel_Nums; i++)
	{
		FMOD::Channel* _Channel = nullptr;
		_CG->getChannel(i, &_Channel);
		_Channel->setPitch(_Pitch->Pitch);
	}
}

void DHSound::PitchDown(Sound_Category _Sound_Category)
{
	// 해당 채널그룹과 피치값을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	Sound_VFP* _Pitch = GetVFP(_Sound_Category);

	// 현재 지정된 피치의 가중치에 따라 증가시켜 적용한다.
	_Pitch->Pitch -= Pitch_Factor;

	if (_Pitch->Pitch < 0) { _Pitch->Pitch = 0.f; }

	// 지정된 채널의 모든 피치를 낮춤.
	int Channel_Nums = 0;
	_CG->getNumChannels(&Channel_Nums);

	for (int i = 0; i < Channel_Nums; i++)
	{
		FMOD::Channel* _Channel = nullptr;
		_CG->getChannel(i, &_Channel);
		_Channel->setPitch(_Pitch->Pitch);
	}
}

void DHSound::PlaySound(Sound_Category _Sound_Category, ATL::CString _Sound_Key)
{
	if (_Sound_Category == Sound_Category::Master)
	{
		printf("[DHSound PlaySound Error] Matser Channel에선 사운드를 재생할 수 없습니다.");
		return;
	}

	if (Sound_Resource.find(_Sound_Key) == Sound_Resource.end())
	{
		printf("[DHSound PlaySound Error] 해당 사운드리소스가 존재하지 않습니다.");
		return;
	}

	// 재생할 사운드를 가져옴.
	FMOD::Sound* _Play_Sound = Sound_Resource[_Sound_Key];
	// 해당 채널그룹을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	// 재생할 채널을 가져옴.
	FMOD::Channel* _Play_Channel = GetChannel();
	// 해당 사운드 재생
	Sound_System->playSound(_Play_Sound, _CG, false, &_Play_Channel);
	// 채널그룹에 채널 등록
	_Play_Channel->setChannelGroup(_CG);
}

void DHSound::StopSound(Sound_Category _Sound_Category)
{
	// 해당 채널그룹을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	// 해당 채널그룹 재생중지.
	_CG->stop();
}

void DHSound::PauseSound(Sound_Category _Sound_Category, bool _Play)
{
	// 해당 채널그룹을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	// 재생 / 일시중지 요청에 따라 실행.
	_CG->setPaused(_Play);
}

void DHSound::Set3DListener(DirectX::XMMATRIX* _View_Matrix, DirectX::XMFLOAT3 _Velocity)
{
	// 카메라의 위치는 뷰 행렬의 역행렬에서 구할 수 있다.
	DirectX::XMFLOAT4X4 Inverse_View_Matrix;
	DirectX::XMVECTOR det;
	XMStoreFloat4x4(&Inverse_View_Matrix, XMMatrixInverse(&det, *_View_Matrix));

	FMOD_3D_ATTRIBUTES listener;
	// 리스너의 위치와 전방 벡터, 상향 벡터를 설정
	listener.position = ConvertFmodVector(DirectX::XMFLOAT3(Inverse_View_Matrix._41, Inverse_View_Matrix._42, Inverse_View_Matrix._43));
	// 뷰 행렬의 역행렬에서 세 번째 행은 전방 벡터
	listener.forward = ConvertFmodVector(DirectX::XMFLOAT3(Inverse_View_Matrix._31, Inverse_View_Matrix._32, Inverse_View_Matrix._33));
	// 뷰 행렬의 역행렬에서 두 번째 행은 상향 벡터
	listener.up = ConvertFmodVector(DirectX::XMFLOAT3(Inverse_View_Matrix._21, Inverse_View_Matrix._22, Inverse_View_Matrix._23));
	// 리스너의 속도 설정. 도플러 효과를 연산하기 위해 필요
	listener.velocity = ConvertFmodVector(_Velocity);
	// FMOD로 보낸다 (0 = 리스너는 하나)
	Sound_System->set3DListenerAttributes(0, &listener.position, &listener.velocity, &listener.forward, &listener.up);
}

void DHSound::Set3DSoundObject(Sound_Category _Sound_Category, ATL::CString _Sound_Key, DirectX::XMFLOAT3 _Position, DirectX::XMFLOAT3 _Velocity)
{
	if (_Sound_Category == Sound_Category::Master)
	{
		printf("[DHSound PlaySound Error] Matser Channel에선 사운드를 재생할 수 없습니다.");
		return;
	}

	if (Sound_Resource.find(_Sound_Key) == Sound_Resource.end())
	{
		printf("[DHSound PlaySound Error] 해당 사운드리소스가 존재하지 않습니다.");
		return;
	}

	// 재생할 사운드를 가져옴.
	FMOD::Sound* _Play_Sound = Sound_Resource[_Sound_Key];
	// 해당 채널그룹을 가져옴.
	FMOD::ChannelGroup* _CG = GetChannelGroup(_Sound_Category);
	// 재생할 채널을 가져옴.
	FMOD::Channel* _Play_Channel = GetChannel();
	// 해당 사운드 재생
	Sound_System->playSound(_Play_Sound, _CG, false, &_Play_Channel);
	// 채널그룹에 채널 등록
	_Play_Channel->setChannelGroup(_CG);
	// 해당 오브젝트의 포지션, 속도값 등록..
	_Play_Channel->set3DAttributes(&ConvertFmodVector(_Position), &ConvertFmodVector(_Velocity));
}

FMOD_VECTOR DHSound::ConvertFmodVector(DirectX::XMFLOAT3 _Vector3)
{
	FMOD_VECTOR()
}
