#pragma once

// FMOD
#ifndef _WIN64
#include "../inc/fmod.hpp"
#pragma comment (lib, "../library/x86/fmod_vc.lib")
using namespace FMOD;
#endif
#ifdef _WIN64
#include "../Resource/inc/fmod.hpp"
#pragma comment (lib, "../Resource/library/_x64/fmod_vc.lib")

using namespace FMOD;
#endif

/// <summary>
/// 사운드 관리하는 매니저
/// BGM과 효과음 따로 enum class로 관리해서
/// </summary>

enum class eSoundChannel
{
	BGM,

	// 플레이어들이 던짐
	Player1UseItem,
	Player2UseItem,
	Player3UseItem,
	Player4UseItem,

	// 플레이어들 상태
	Player1State,
	Player2State,
	Player3State,
	Player4State,

	// 맵 상태
	MapState,

	//기차
	Train,
	TrainSignal,

	ReadySetGo,

	Size
};

enum class eSoundList
{
	BGMStart,
	TitleBGM,
	GameSettingBGM,
	Map1BGM,
	BGMEnd,

	EffectStart,
	BottleEffect,
	PunchEffect,
	ElectricEffect,
	SnowballEffect,
	ShotEffectEnd,
	HoleEffect,
	SpeedEffect,
	TrapEffect,
	WaveEffect,
	HideEffect,

	UICancleEffect,
	UIClickEffect,
	UIMoveEffect,
	StunEffect,
	ThrowEffect,
	PickUpEffect,
	KeepEffect,
	SetupEffect,

	ItemBoxEffect,
	SignalEffect,

	TruckEffect,
	TrainEffect,
	WantedEffect,
	EffectEnd,

	GoEffect,
	ReadyEffect,

	Size
};

class SoundManager
{
public:
	static SoundManager* GetInstance();

	void Initialize();
	void Finalize() const;

	void LoadMusic(eSoundList soundList, bool loopType, const std::string& sound);
	void PlayMusic(eSoundList soundList, eSoundChannel channel, float volume);
	void StopMusic(eSoundChannel channel) const;
	void SetVolume(float volume, eSoundChannel channel) const;

private:
	static SoundManager* m_pInstance;

private:
	FMOD::System* m_pSystem = nullptr;
	FMOD::Channel* m_pChannel[static_cast<int>(eSoundChannel::Size)] = {};
	FMOD::Sound* m_pSoundList[static_cast<int>(eSoundList::Size)] = {};
};