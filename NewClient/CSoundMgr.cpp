#include"pch.h"
#include "SoundMgr.h"

void Sound::SoundPlay(bool IsLoop)
{
    if (m_pChannel == nullptr)
    {
        if(IsLoop) m_pSound->setMode(FMOD_LOOP_NORMAL);
        else  m_pSound->setMode(FMOD_LOOP_OFF);
        m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
        m_pChannel->setVolume(0.1);
    }
    else
    {
        bool play;
        m_pChannel->isPlaying(&play);
        if (play)
        {
            return;
        }
        else
        {
            if (IsLoop) m_pSound->setMode(FMOD_LOOP_NORMAL);
            else  m_pSound->setMode(FMOD_LOOP_OFF);
            m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
            m_pChannel->setVolume(0.1);
        }

    }
  
}

void Sound::EffectSoundPlay()
{
    m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
    m_pChannel->setVolume(0.1);
}

void Sound::SoundStop()
{
    m_pChannel->stop();

}

void Sound::SooundPause()
{
    bool IsPause;

    m_pChannel->getPaused(&IsPause);
    m_pChannel->setPaused(!IsPause);

}

bool Sound::Load(std::wstring filename)
{
    std::string file = wtm(filename);
    m_pSystem->createSound(file.c_str(),FMOD_DEFAULT,0,&m_pSound);
    return true;
}

bool Sound::Init()
{

    return true;
}

bool Sound::Frame()
{
    return true;
}

bool Sound::Release()
{
    if (m_pSound)m_pSound->release();
    return true;
}

std::shared_ptr<Sound> SoundMgr::Load(std::wstring FileName)
{
    std::size_t found = FileName.find_last_of(L"/");
    std::wstring path = FileName.substr(0, found + 1);
    std::wstring Key = FileName.substr(found + 1);
    std::shared_ptr<Sound> data = GetPtr(Key);

    if (data != nullptr)
    {
        return data;

    }
    std::shared_ptr<Sound> NewData = std::make_shared<Sound>();
    NewData->Set(m_pSystem);
    NewData->SetName(Key);

    if (NewData->Load(FileName))
    {
        m_List.insert(std::make_pair(Key, std::move(NewData)));
        return GetPtr(Key);
    }

    
    return nullptr;
}

std::shared_ptr<Sound> SoundMgr::GetPtr(std::wstring FileName)
{
    auto iter = m_List.find(FileName);
    if (m_List.end() == iter)
    {
        return nullptr;
    }

    return iter->second;

}

bool SoundMgr::Get(std::wstring FileName, std::shared_ptr<Sound> sound)
{
    auto iter = m_List.find(FileName);
    if (m_List.end() == iter)
    {
        return false;
    }
    sound = iter->second;

    return true;
}



bool SoundMgr::Frame()
{
    m_pSystem->update();
    for (auto& data : m_List)
    {
        data.second->Frame();
    }
    return true;
   
}

bool SoundMgr::Release()
{
    for (auto& data : m_List)
    {
        data.second->Release();
    }
    m_List.clear();

    m_pSystem->close();
    m_pSystem->release();
    return true;
}



