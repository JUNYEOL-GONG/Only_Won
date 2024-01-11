#include "pch.h"
#include "SoundManager.h"

SoundManager* SoundManager::m_pInstance = nullptr;

SoundManager* SoundManager::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new SoundManager;
	return m_pInstance;
}

void SoundManager::Initialize()
{
	System_Create(&m_pSystem);
	m_pSystem->init(static_cast<int>(eSoundChannel::Size), FMOD_INIT_NORMAL, nullptr);

	LoadMusic(eSoundList::TitleBGM, true, "../Resource/BGM_Title.wav");
	LoadMusic(eSoundList::GameSettingBGM, true, "../Resource/BGM_Gamesetting.wav");
	LoadMusic(eSoundList::Map1BGM, true, "../Resource/BGM_Map1.wav");

	LoadMusic(eSoundList::BottleEffect, false, "../Resource/SE_Bottle.wav");
	LoadMusic(eSoundList::PunchEffect, false, "../Resource/SE_Punch.wav");
	LoadMusic(eSoundList::ElectricEffect, false, "../Resource/SE_Electric.wav");
	LoadMusic(eSoundList::SnowballEffect, false, "../Resource/SE_Snowball.wav");

	LoadMusic(eSoundList::HoleEffect, false, "../Resource/SE_Hole.mp3");
	LoadMusic(eSoundList::SpeedEffect, false, "../Resource/SE_Speed.wav");
	LoadMusic(eSoundList::TrapEffect, false, "../Resource/SE_Trap.wav");
	LoadMusic(eSoundList::WaveEffect, false, "../Resource/SE_Wave.wav");
	LoadMusic(eSoundList::HideEffect, false, "../Resource/SE_Hide.mp3");

	LoadMusic(eSoundList::UICancleEffect, false, "../Resource/SE_UICancle.wav");
	LoadMusic(eSoundList::UIClickEffect, false, "../Resource/SE_UIClick.wav");

	LoadMusic(eSoundList::StunEffect, false, "../Resource/SE_Stun.wav");
	LoadMusic(eSoundList::ThrowEffect, false, "../Resource/SE_Throw.mp3");
	LoadMusic(eSoundList::PickUpEffect, false, "../Resource/SE_Pickup.wav");
	LoadMusic(eSoundList::KeepEffect, false, "../Resource/SE_Keep.wav");
	LoadMusic(eSoundList::SetupEffect, false, "../Resource/SE_Setup.wav");

	LoadMusic(eSoundList::ItemBoxEffect, false, "../Resource/SE_Item.wav");
	LoadMusic(eSoundList::SignalEffect, false, "../Resource/SE_Signal.wav");

	LoadMusic(eSoundList::TruckEffect, false, "../Resource/SE_Truck.wav");
	LoadMusic(eSoundList::WantedEffect, false, "../Resource/SE_Wanted.wav");

	LoadMusic(eSoundList::GoEffect, false, "../Resource/SE_Go.wav");
	LoadMusic(eSoundList::ReadyEffect, false, "../Resource/SE_Ready.wav");

	LoadMusic(eSoundList::TrainEffect, false, "../Resource/SE_Train.wav");
}

void SoundManager::Finalize() const
{
	delete m_pInstance;

	// 자원 반환
	m_pSystem->close();
	m_pSystem->release();

}

void SoundManager::LoadMusic(eSoundList soundList, bool loopType, const std::string& sound)
{
	if (loopType)
	{
		m_pSystem->createSound(sound.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_pSoundList[static_cast<int>(soundList)]);
	}
	else
	{
		m_pSystem->createSound(sound.c_str(), FMOD_INIT_NORMAL, nullptr, &m_pSoundList[static_cast<int>(soundList)]);
	}
}

void SoundManager::PlayMusic(eSoundList soundlist, eSoundChannel channel, float volume)
{
	m_pSystem->playSound(m_pSoundList[static_cast<int>(soundlist)], nullptr, false, &m_pChannel[static_cast<int>(channel)]);
	m_pChannel[static_cast<int>(channel)]->setVolume(volume);
}

void SoundManager::StopMusic(eSoundChannel channel) const
{
	m_pChannel[static_cast<int>(channel)]->stop();
}

void SoundManager::SetVolume(float volume, eSoundChannel channel) const
{
	m_pChannel[static_cast<int>(channel)]->setVolume(volume);
}