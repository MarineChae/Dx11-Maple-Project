#pragma once
#include"Singleton.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment( lib, "fmod_vc.lib")

class Sound
{
public:
	std::wstring m_FileName;
	std::wstring m_FilePath;

public:

	FMOD::System* m_pSystem;
	FMOD::Sound* m_pSound;
	FMOD::Channel* m_pChannel;
public:
	void Set(FMOD::System* pSystem)	{ m_pSystem = pSystem; }
	void SoundPlay(bool IsLoop);
	void EffectSoundPlay();
	void SoundStop();
	void SooundPause();
	void SetName(std::wstring Key){	m_FileName = Key;}

public:

	bool Load(std::wstring filename);
	bool Init();
	bool Frame();
	bool Release();
};
class SoundMgr : public Singleton<SoundMgr>
{
public:
	FMOD::System* m_pSystem;
	std::map < std::wstring, std::shared_ptr<Sound>> m_List;
public:
	
	std::shared_ptr<Sound> Load(std::wstring FileName);
	std::shared_ptr<Sound> GetPtr(std::wstring FileName);
	bool Get(std::wstring FileName, std::shared_ptr<Sound> sound);
	bool Frame();
	bool Release();

	SoundMgr()
	{
		FMOD::System_Create(&m_pSystem);
		m_pSystem->init(64, FMOD_INIT_NORMAL, 0);

	}

};
