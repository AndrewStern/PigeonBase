//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "MediaStreamer.h"
#include "GameObject.h"

static const int STREAMING_BUFFER_SIZE = 65536;
static const int MAX_BUFFER_COUNT = 3;

enum SoundEffectType
{
	Single,
	Continous,
	Falling,
	CheckPoint,
	Collision,
	MenuChanged,
	Backgound,
};

struct SoundEffectData
{
	SoundEffectType				m_type;
	GameObject*					m_object;
	WCHAR*						m_filename;
    IXAudio2SourceVoice*        m_soundEffectSourceVoice;
    XAUDIO2_BUFFER              m_audioBuffer;
    byte*                       m_soundEffectBufferData;
    uint32                      m_soundEffectBufferLength;
    uint32                      m_soundEffectSampleRate;
    bool                        m_soundEffectStarted;
};

struct StreamingVoiceContext : public IXAudio2VoiceCallback
{
    STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32){}
    STDMETHOD_(void, OnVoiceProcessingPassEnd)(){}
    STDMETHOD_(void, OnStreamEnd)(){}
    STDMETHOD_(void, OnBufferStart)(void*)
    {
        ResetEvent(hBufferEndEvent);
    }
    STDMETHOD_(void, OnBufferEnd)(void* pContext)
    {
        // Trigger the event for the music stream.
        if (pContext == 0) {
            SetEvent(hBufferEndEvent);
        }
    }
    STDMETHOD_(void, OnLoopEnd)(void*){}
    STDMETHOD_(void, OnVoiceError)(void*, HRESULT){}

    HANDLE hBufferEndEvent;
    StreamingVoiceContext() : hBufferEndEvent(CreateEventEx(NULL, FALSE, FALSE, NULL))
    {
    }
    virtual ~StreamingVoiceContext()
    {
        CloseHandle(hBufferEndEvent);
    }
};

class AudioEngine;
//class Sounds;
class AudioEngineCallbacks: public IXAudio2EngineCallback
{
private:
    AudioEngine* m_audio;

public :
    AudioEngineCallbacks(){};
    void Initialize(AudioEngine* audio);

    // Called by XAudio2 just before an audio processing pass begins.
    void _stdcall OnProcessingPassStart(){};

    // Called just after an audio processing pass ends.
    void  _stdcall OnProcessingPassEnd(){};

    // Called when a critical system error causes XAudio2
    // to be closed and restarted. The error code is given in Error.
    void  _stdcall OnCriticalError(HRESULT Error);
};

class AudioEngine
{
private:
    IXAudio2*									m_musicEngine;
    IXAudio2*									m_soundEffectEngine;
    IXAudio2MasteringVoice*						m_musicMasteringVoice;
    IXAudio2MasteringVoice*						m_soundEffectMasteringVoice;
    IXAudio2SourceVoice*						m_musicSourceVoice;
    StreamingVoiceContext						m_voiceContext;
    byte										m_audioBuffers[MAX_BUFFER_COUNT][STREAMING_BUFFER_SIZE];
    MediaStreamer								m_musicStreamer;
    uint32										m_currentBuffer;
    std::list<SoundEffectData>					m_soundEffects;
	std::list<SoundEffectData>::iterator		m_soundEffectsIterator;
    XAUDIO2FX_REVERB_PARAMETERS					m_reverbParametersLarge;
    XAUDIO2FX_REVERB_PARAMETERS					m_reverbParametersSmall;
    IXAudio2SubmixVoice*						m_soundEffectReverbVoiceSmallRoom;
    IXAudio2SubmixVoice*						m_soundEffectReverbVoiceLargeRoom;
    IXAudio2SubmixVoice*						m_musicReverbVoiceSmallRoom;
    IXAudio2SubmixVoice*						m_musicReverbVoiceLargeRoom;
    bool										m_engineExperiencedCriticalError;
    AudioEngineCallbacks						m_musicEngineCallback;
    AudioEngineCallbacks						m_soundEffectEngineCallback;
	WCHAR*										m_Background;

    void CreateReverb(IXAudio2* engine, IXAudio2MasteringVoice* masteringVoice, XAUDIO2FX_REVERB_PARAMETERS* parameters, IXAudio2SubmixVoice** newSubmix, bool enableEffect);

public:
	bool m_isAudioStarted;

    void Initialize();
    void CreateResources();
    void ReleaseResources();
	void CreateSourceVoice(GameObject*, WCHAR*, SoundEffectType);
    void Start();
    void Render();
    void SuspendAudio();
    void ResumeAudio();

    // This flag can be used to tell when the audio system
    // is experiencing critial errors.
    // XAudio2 gives a critical error when the user unplugs
    // the headphones and a new speaker configuration is generated.
    void SetEngineExperiencedCriticalError()
    {
        m_engineExperiencedCriticalError = true;
    }

    bool HasEngineExperiencedCriticalError()
    {
        return m_engineExperiencedCriticalError;
    }

    void PlaySoundEffect(GameObject*, WCHAR*);
    bool IsSoundEffectStarted(GameObject*, WCHAR*);
    void StopSoundEffect(GameObject*, WCHAR*);
    void SetSoundEffectVolume(GameObject*, WCHAR*, float volume);
    void SetSoundEffectPitch(GameObject*, WCHAR*, float pitch);
    void SetSoundEffectFilter(GameObject*, WCHAR*, float frequency, float oneOverQ);
	void SetBackgroundVolume(float volume);
    void SetBackgroundPitch(float pitch);
    void SetBackgroundFilter(float frequency, float oneOverQ, uint32 samplerate);
    void SetRoomSize(float roomSize, float* wallDistances);
	void CreateBackgroundMusic();
};

