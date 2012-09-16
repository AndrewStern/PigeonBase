#include "pch.h"
#include "AudioManager.h"
#include "AudioEngine.h"


void AudioManager::Initialize(AudioEngine* engine)
{
	p_Audio = engine;
	if(p_Audio)
	{
		p_Audio->SetManager(this);
	}
}

AudioEntry * AudioManager::AddSound(wchar_t* _SoundName, int _Type)
{	
	wchar_t* m_filename = L"Assets/Audio/";
	int a = wcslen(m_filename) + wcslen(_SoundName) + 1;
	wchar_t* buf = new wchar_t[a];
	wcscpy_s(buf, a, m_filename);
	wcscat_s(buf, a, _SoundName);
	bool exists = false;
	AudioEntry* audio = nullptr;
	for(m_audioStoreIterator = m_audioStore.begin(); m_audioStoreIterator != m_audioStore.end(); ++m_audioStoreIterator)
	{
		if(wcscmp(m_audioStoreIterator->m_soundName, _SoundName) == 0)
		{
			exists = true;
			m_audioStoreIterator->m_refCount++;
			audio = (AudioEntry*)&m_audioStoreIterator;
		}
	}
	if (!exists)
	{
		audio = new AudioEntry();
		audio->m_refCount = 1;
		audio->m_soundName = buf;
		audio->m_type = _Type;
		m_audioStore.push_back(*audio);
	}
	return audio;
}

void AudioManager::DeleteSound(wchar_t* _SoundName)
{
	
}

void AudioManager::PlaySound(wchar_t* _SoundName)
{
	wchar_t* m_filename = L"Assets/Audio/";
	int a = wcslen(m_filename) + wcslen(_SoundName) + 1;
	wchar_t* buf = new wchar_t[a];
	wcscpy_s(buf, a, m_filename);
	wcscat_s(buf, a, _SoundName);
	for(m_audioStoreIterator = m_audioStore.begin(); m_audioStoreIterator != m_audioStore.end(); ++m_audioStoreIterator)
	{
		if(wcscmp(m_audioStoreIterator->m_soundName, buf) == 0)
		{
			if(p_Audio)
			{
				p_Audio->PlaySoundEffect(buf);
			} 
		}
	}	
}

void AudioManager::StopSound(wchar_t* _SoundName)
{
	wchar_t* m_filename = L"Assets/Audio/";
	int a = wcslen(m_filename) + wcslen(_SoundName) + 1;
	wchar_t* buf = new wchar_t[a];
	wcscpy_s(buf, a, m_filename);
	wcscat_s(buf, a, _SoundName);
	for(m_audioStoreIterator = m_audioStore.begin(); m_audioStoreIterator != m_audioStore.end(); ++m_audioStoreIterator)
	{
		if(wcscmp(m_audioStoreIterator->m_soundName, buf) == 0)
		{
			if(p_Audio)
			{
				p_Audio->StopSoundEffect(buf);
			} 

		}
	}
}

void AudioManager::AddMusic(wchar_t* _MusicName)
{	
	wchar_t* m_filename = L"Assets/Audio/";
	int a = wcslen(m_filename) + wcslen(_MusicName) + 1;
	wchar_t* buf = new wchar_t[a];
	wcscpy_s(buf, a, m_filename);
	wcscat_s(buf, a, _MusicName);
	
	m_BackroundMusic = buf;
}

void AudioManager::SetBackgroundVolume(float volume)
{
}

void AudioManager::SetBackGroundPitch(float pitch)
{
}

void AudioManager::SetBackGroundFilter(float frequency, float oneOverQ, uint32 samplerate)
{
}
