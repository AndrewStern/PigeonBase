//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "MediaStreamer.h"
#include "GameObject.h"
#include "SoundEffect.h"
#include "AudioManager.h"

class SoundEffect;
class AudioManager;

static const int STREAMING_BUFFER_SIZE = 65536;
static const int MAX_BUFFER_COUNT = 3;

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
        if (pContext == 0) 
		{
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

class AudioEngineCallbacks: public IXAudio2EngineCallback
{
private:
    AudioEngine* m_audio;

public :
    AudioEngineCallbacks(){};
    void Initialize(AudioEngine* audio);

    /** Called by XAudio2 just before an audio processing pass begins. */
    void _stdcall OnProcessingPassStart(){};

    /** Called just after an audio processing pass ends. */
    void  _stdcall OnProcessingPassEnd(){};

    /** 
	 * Called when a critical system error causes XAudio2
     * to be closed and restarted. The error code is given in Error.
	 */
    void  _stdcall OnCriticalError(HRESULT Error);
};


/**
 * This is the Audio Engine class that handles all the audio related functionalities such as playing music tracks, 
 * playing sounds, adding effects, modifying volumes, stopping sounds/music etc.
 */
class AudioEngine
{
private:
    IXAudio2*									m_musicEngine;													/**  */
    IXAudio2*									m_soundEffectEngine;											/**  */
    IXAudio2MasteringVoice*						m_musicMasteringVoice;											/**  */
    IXAudio2MasteringVoice*						m_soundEffectMasteringVoice;									/**  */
    IXAudio2SourceVoice*						m_musicSourceVoice;												/**  */
    StreamingVoiceContext						m_voiceContext;													/**  */
    byte										m_audioBuffers[MAX_BUFFER_COUNT][STREAMING_BUFFER_SIZE];		/**  */
    MediaStreamer								m_musicStreamer;												/** A MediaStreamer object used for streaming audio, like the background music */
    uint32										m_currentBuffer;												/** Holds the current buffer value */
	std::list<SoundEffect>						m_soundEffects;													/** A list of all the sounds currently relevant in the game, that the audio engine can be requested to play at any time. */
	std::list<SoundEffect>::iterator			m_soundEffectsIterator;											/** An iterator for traversing through the m_soundEffects list */
    XAUDIO2FX_REVERB_PARAMETERS					m_reverbParametersLarge;										/**  */
    XAUDIO2FX_REVERB_PARAMETERS					m_reverbParametersSmall;										/**  */
    IXAudio2SubmixVoice*						m_soundEffectReverbVoiceSmallRoom;								/**  */
    IXAudio2SubmixVoice*						m_soundEffectReverbVoiceLargeRoom;								/**  */
    IXAudio2SubmixVoice*						m_musicReverbVoiceSmallRoom;									/**  */
    IXAudio2SubmixVoice*						m_musicReverbVoiceLargeRoom;									/**  */
    bool										m_engineExperiencedCriticalError;								/** A falg used to test whether all the engine experience an error while performing its operations */
    AudioEngineCallbacks						m_musicEngineCallback;											/**  */
    AudioEngineCallbacks						m_soundEffectEngineCallback;									/**  */
	WCHAR*										m_Background;													/** Filename of the Background Track  */
	AudioManager*								m_AudioManager;													/** Reference to the Main's AudioManager */

    void CreateReverb(IXAudio2* engine, IXAudio2MasteringVoice* masteringVoice, XAUDIO2FX_REVERB_PARAMETERS* parameters, IXAudio2SubmixVoice** newSubmix, bool enableEffect);

public:
	/** A flag that indicates if the audio stream for the music hass started or not */
	bool m_isAudioStarted;

	/**
	 * Creates a voice for a sound effect 
	 *
	 * @param		filename		The filename of the Audio file
	 * @param		type			The type of the sound effect. 0 = Single, 1 = Continous, default = Single;
	 */
	void CreateSourceVoice(WCHAR* filename, int type);

	/**
	 * Initialises all the variables to their default values
	 */
    void Initialize();

	/**
	 * Creates all the sound resources
	 */
    void CreateResources();

	/**
	 * Releases all the resources the engine hold currently
	 */
    void ReleaseResources();

	/**
	 * Starts the background stream
	 */
    void Start();

	/**
	 * Renders the background music
	 */
    void Render();

	/**
	 * Suspend the Audio Engine
	 */
    void SuspendAudio();

	/**
	 * Resumes the Audio Engine
	 */
    void ResumeAudio();

	/**
	 * Sets a reference to the AudioManager
	 */
	void SetManager(AudioManager* _Audio){m_AudioManager = _Audio;}

    /**
	 * This flag can be used to tell when the audio system
     * is experiencing critial errors.
     * XAudio2 gives a critical error when the user unplugs
     * the headphones and a new speaker configuration is generated.
	 */
    void SetEngineExperiencedCriticalError()
    {
        m_engineExperiencedCriticalError = true;
    }

	/**
	 * This function is used to find out the value of the m_engineExperiencedCriticalError flag
	 */
    bool HasEngineExperiencedCriticalError()
    {
        return m_engineExperiencedCriticalError;
    }

	/**
	 * Play the specified sound effect
	 *
	 * @param		filename		The name of the sound effect to play
	 */
    void PlaySoundEffect(WCHAR* filename);

	/**
	 * Returns the state of a given sound. Useful to check before trying to stop a sound.
	 *
	 * @param		filename		Sound effect name to check against
	 * @return		true = sound effect started, false = sound effect not started
	 */
    bool IsSoundEffectStarted(WCHAR* filename);

	/**
	 * Stops playing the specified sound. 
	 *
	 * @param		filename		Sound effect name to stop playing
	 */
    void StopSoundEffect(WCHAR* filename);

	/**
	 * Sets the volume of the specified sound effect
	 *
	 * @param		filename		The sound effect to set the volume of.
	 * @param		volume			The volume to set for the sound effect.
	 */
    void SetSoundEffectVolume(WCHAR* filename, float volume);

	/**
	 * Sets the pitch of the specified sound effect
	 *
	 * @param		filename		The sound effect to set the pitch of.
	 * @param		pitch			The pitch to set for the sound effect.
	 */
    void SetSoundEffectPitch(WCHAR* filename, float pitch);

	/**
	 * Sets the filter to the specified sound effect
	 *
	 * @param		filename			The sound effect to apply the filter to.
	 * @param		frequency			The frequency value to apply to the sound effect.
	 * @param		oneOverQ			
	 */
    void SetSoundEffectFilter(WCHAR* filename, float frequency, float oneOverQ);

	/**
	 * Sets the volume of the Background music
	 *
	 * @param		volume			The volume to set for the background music.
	 */
	void SetBackgroundVolume(float volume);

	/**
	 * Sets the volume of the Background Image
	 *
	 * @param		pitch			The pitch to set for the background music.
	 */
    void SetBackgroundPitch(float pitch);

	/**
	 * Sets the filter to the specified sound effect
	 *
	 * @param		frequency			The frequency value to apply to the sound effect.
	 * @param		oneOverQ	
	 * @param		samplerate			The samplerate to apply to the Background music
	 */
    void SetBackgroundFilter(float frequency, float oneOverQ, uint32 samplerate);

	/**
	 * Gets the curent volumeof the background music
	 *
	 * @return		Volume of the background music
	 */
	float GetBackgroundVolume();

	/**
	 * Gets the current volume of the specified sound effect
	 *
	 * @param		filename		The filename of the sound effect to retriev the volume information from.
	 * @return		The volume of the specified soundeffect
	 */
	float GetSoundEffectVolume(WCHAR* filename);

	/**
	 *	Sets the size of the current room
	 */
    void SetRoomSize(float roomSize, float* wallDistances);

	/**
	 * Creates the Background music stream and its master voice for playing the background
	 */
	void CreateBackgroundMusic();

	/**
	 * Creates the streams and voices of all the sound effects
	 */
	void CreateSoundEffects();
};

