//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "DirectXHelper.h"
#include "AudioEngine.h"

using namespace _2D_Game_Engine;

void AudioEngineCallbacks::Initialize(AudioEngine* audio)
{
    m_audio = audio;
}

// Called when a critical system error causes XAudio2
// to be closed and restarted. The error code is given in Error.
void  _stdcall AudioEngineCallbacks::OnCriticalError(HRESULT Error)
{
    m_audio->SetEngineExperiencedCriticalError();
}

void AudioEngine::Initialize()
{
    m_isAudioStarted = false;
    m_musicEngine = nullptr;
    m_soundEffectEngine = nullptr;
    m_musicMasteringVoice = nullptr;
    m_soundEffectMasteringVoice = nullptr;
    m_musicSourceVoice = nullptr;
	m_Background = L"";

    m_reverbParametersSmall.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
    m_reverbParametersSmall.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
    m_reverbParametersSmall.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
    m_reverbParametersSmall.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
    m_reverbParametersSmall.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
    m_reverbParametersSmall.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
    m_reverbParametersSmall.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
    m_reverbParametersSmall.EarlyDiffusion = 4;
    m_reverbParametersSmall.LateDiffusion = 15;
    m_reverbParametersSmall.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
    m_reverbParametersSmall.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
    m_reverbParametersSmall.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
    m_reverbParametersSmall.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
    m_reverbParametersSmall.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
    m_reverbParametersSmall.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
    m_reverbParametersSmall.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
    m_reverbParametersSmall.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
    m_reverbParametersSmall.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
    m_reverbParametersSmall.DecayTime = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME;
    m_reverbParametersSmall.Density = XAUDIO2FX_REVERB_DEFAULT_DENSITY;
    m_reverbParametersSmall.RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
    m_reverbParametersSmall.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;
    m_reverbParametersSmall.DisableLateField = TRUE;

    m_reverbParametersLarge.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
    m_reverbParametersLarge.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
    m_reverbParametersLarge.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
    m_reverbParametersLarge.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
    m_reverbParametersLarge.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
    m_reverbParametersLarge.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
    m_reverbParametersLarge.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
    m_reverbParametersLarge.EarlyDiffusion = 4;
    m_reverbParametersLarge.LateDiffusion = 4;
    m_reverbParametersLarge.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
    m_reverbParametersLarge.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
    m_reverbParametersLarge.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
    m_reverbParametersLarge.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
    m_reverbParametersLarge.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
    m_reverbParametersLarge.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
    m_reverbParametersLarge.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
    m_reverbParametersLarge.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
    m_reverbParametersLarge.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
    m_reverbParametersLarge.DecayTime = 1.0f;
    m_reverbParametersLarge.Density = 10;
    m_reverbParametersLarge.RoomSize = 100;
    m_reverbParametersLarge.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;
    m_reverbParametersLarge.DisableLateField = FALSE;

	for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
    {
		m_soundEffectsIterator->m_soundEffectBufferData = nullptr;
        m_soundEffectsIterator->m_soundEffectSourceVoice = nullptr;
        m_soundEffectsIterator->m_soundEffectStarted = false;
		ZeroMemory(&m_soundEffectsIterator->m_audioBuffer,sizeof(m_soundEffectsIterator->m_audioBuffer));
    }
    ZeroMemory(m_audioBuffers, sizeof(m_audioBuffers));
}

void AudioEngine::SetRoomSize(float roomSize, float* wallDistances)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
	
	HRESULT hr;
	// Maximum "outdoors" is a room size of 1000.0f
	float normalizedRoomSize = roomSize  / 1000.0f;

	float outputMatrix[4] = {0, 0, 0, 0};
	float leftRatio = wallDistances[2] / (wallDistances[2] + wallDistances[3]);
	if ((wallDistances[2] + wallDistances[3]) == 0)
	{
		leftRatio = 0.5f;
	}
	float width = abs((wallDistances[3] + wallDistances[5] + wallDistances[7]) / 3.0f - (wallDistances[2] + wallDistances[4] + wallDistances[6]) / 3.0f);
	float height = abs(wallDistances[1] - wallDistances[0]);
	float widthNormalized = width / max(width, height);
	if (width == 0)
	{
		widthNormalized = 1.0f;
	}

	// As widthNormalized approaches 0, the room is taller than wide.
	// This means that the late-field reverb should be non-localized across both speakers.
	leftRatio -= .5f;
	leftRatio *= widthNormalized;
	leftRatio += .5f;

	// Near-field reverb with the speaker closest to the wall getting more reverb
	outputMatrix[0] = (1.0f - leftRatio)* normalizedRoomSize;
	outputMatrix[1] = 0;
	outputMatrix[2] = 0;
	outputMatrix[3] = leftRatio * normalizedRoomSize;
	
	hr = m_soundEffectReverbVoiceSmallRoom->SetOutputMatrix(m_soundEffectMasteringVoice, 2, 2, outputMatrix);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	outputMatrix[0] = (1.0f - leftRatio) * normalizedRoomSize;
	outputMatrix[1] = 0;
	outputMatrix[2] = 0;
	outputMatrix[3]= leftRatio * normalizedRoomSize;
	
	hr = m_musicReverbVoiceSmallRoom->SetOutputMatrix(m_musicMasteringVoice, 2, 2, outputMatrix);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	// Late-field reverb: The wall furthest away gets more of the late-field reverb
	outputMatrix[0] = leftRatio * (1.0f - normalizedRoomSize);
	outputMatrix[1] = 0;
	outputMatrix[2] = 0;
	outputMatrix[3]= (1.0f - leftRatio) * (1.0f - normalizedRoomSize);
	
	hr = m_soundEffectReverbVoiceLargeRoom->SetOutputMatrix(m_soundEffectMasteringVoice, 2, 2, outputMatrix);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	outputMatrix[0] = leftRatio * (1.0f - normalizedRoomSize);
	outputMatrix[1] = 0;
	outputMatrix[2] = 0;
	outputMatrix[3] = (1.0f - leftRatio) * (1.0f - normalizedRoomSize);

	hr = m_musicReverbVoiceLargeRoom->SetOutputMatrix(m_musicMasteringVoice, 2, 2, outputMatrix);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
	}
}

void AudioEngine::CreateResources()
{
	try
	{
   		CreateBackgroundMusic();
		CreateSoundEffects();
	}
	catch(...)
	{
		m_engineExperiencedCriticalError = true;
	}
}

void AudioEngine::CreateReverb(IXAudio2* engine, IXAudio2MasteringVoice* masteringVoice, XAUDIO2FX_REVERB_PARAMETERS* parameters, IXAudio2SubmixVoice** newSubmix, bool enableEffect)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}

	HRESULT hr;

	XAUDIO2_EFFECT_DESCRIPTOR soundEffectdescriptor;
	XAUDIO2_EFFECT_CHAIN soundEffectChain;
	Microsoft::WRL::ComPtr<IUnknown> soundEffectXAPO;

	hr = XAudio2CreateReverb(&soundEffectXAPO);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	soundEffectdescriptor.InitialState = false;
	soundEffectdescriptor.OutputChannels = 2;
	soundEffectdescriptor.pEffect = soundEffectXAPO.Get();
	soundEffectChain.EffectCount = 1;
	soundEffectChain.pEffectDescriptors = &soundEffectdescriptor;

	hr = engine->CreateSubmixVoice(newSubmix, 2, 48000, 0, 0, nullptr, &soundEffectChain);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	hr = (*newSubmix)->SetEffectParameters(0, parameters, sizeof(m_reverbParametersSmall));
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	if (enableEffect)
	{
		hr = (*newSubmix)->EnableEffect(0);
		if(FAILED(hr))
		{
			m_engineExperiencedCriticalError = true;
			return;
		}
	}

	hr = (*newSubmix)->SetVolume (1.0f);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	float outputMatrix[4] = {0, 0, 0, 0};
	hr = (*newSubmix)->SetOutputMatrix(masteringVoice, 2, 2, outputMatrix);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
	}
}

void AudioEngine::CreateSourceVoice(WCHAR* filename, int type)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
	
	if(type != 0 && type != 1)
	{
		type = 0;
	}
	// Load all data for each sound effect into a single in-memory buffer
	MediaStreamer soundEffectStream;

	SoundEffect temp;
	//temp.m_object = object;
	temp.m_filename = filename;
	temp.m_type = SoundEffectType(type);

	std::wstring test = filename;
	soundEffectStream.Initialize(filename); 

	uint32 bufferLength = soundEffectStream.GetMaxStreamLengthInBytes();
	temp.m_soundEffectBufferData = new byte[bufferLength];
	soundEffectStream.ReadAll(temp.m_soundEffectBufferData, bufferLength, &temp.m_soundEffectBufferLength);


	XAUDIO2_SEND_DESCRIPTOR descriptors[3];
	descriptors[0].pOutputVoice = m_soundEffectMasteringVoice;
	descriptors[0].Flags = 0;
	descriptors[1].pOutputVoice = m_soundEffectReverbVoiceSmallRoom;
	descriptors[1].Flags = 0;
	descriptors[2].pOutputVoice = m_soundEffectReverbVoiceLargeRoom;
	descriptors[2].Flags = 0;
	XAUDIO2_VOICE_SENDS sends = {0};
	sends.SendCount = 3;
	sends.pSends = descriptors;

	HRESULT hr;
	// The Continuous sound can have pitch shifting and a low-pass filter.
	if (temp.m_type == SoundEffectType::Continous)
	{

		hr = m_soundEffectEngine->CreateSourceVoice(
				&temp.m_soundEffectSourceVoice,
				&(soundEffectStream.GetOutputWaveFormatEx()),
				XAUDIO2_VOICE_USEFILTER,
				2.0f,
				&m_voiceContext,
				&sends);

	}
	else
	{
	
		hr = m_soundEffectEngine->CreateSourceVoice(
				&temp.m_soundEffectSourceVoice,
				&(soundEffectStream.GetOutputWaveFormatEx()),
				0,
				1.0f,
				&m_voiceContext,
				&sends);

	}
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}
	temp.m_soundEffectSampleRate = soundEffectStream.GetOutputWaveFormatEx().nSamplesPerSec;

	// Queue in-memory buffer for playback
	ZeroMemory(&temp.m_audioBuffer, sizeof(temp.m_audioBuffer));

	temp.m_audioBuffer.AudioBytes = temp.m_soundEffectBufferLength;
	temp.m_audioBuffer.pAudioData = temp.m_soundEffectBufferData;
	temp.m_audioBuffer.pContext = &temp;
	temp.m_audioBuffer.Flags = XAUDIO2_END_OF_STREAM;
	if (temp.m_type == SoundEffectType::Continous)
	{
		temp.m_audioBuffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
		temp.m_audioBuffer.LoopCount = 0;
	}


	hr = temp.m_soundEffectSourceVoice->SubmitSourceBuffer(&temp.m_audioBuffer);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	m_soundEffects.push_back(temp);

}

void AudioEngine::ReleaseResources()
{
    if (m_musicSourceVoice != nullptr)
    {
        m_musicSourceVoice->DestroyVoice();
    }
    if (m_soundEffectReverbVoiceSmallRoom != nullptr)
    {
        m_soundEffectReverbVoiceSmallRoom->DestroyVoice();
    }
    if (m_soundEffectReverbVoiceLargeRoom != nullptr)
    {
        m_soundEffectReverbVoiceLargeRoom->DestroyVoice();
    }
    if (m_musicReverbVoiceSmallRoom != nullptr)
    {
        m_musicReverbVoiceSmallRoom->DestroyVoice();
    }
    if (m_musicReverbVoiceLargeRoom != nullptr)
    {
        m_musicReverbVoiceLargeRoom->DestroyVoice();
    }
    for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
    {
        if (m_soundEffectsIterator->m_soundEffectSourceVoice != nullptr)
        {
            m_soundEffectsIterator->m_soundEffectSourceVoice->DestroyVoice();
        }
        m_soundEffectsIterator->m_soundEffectSourceVoice = nullptr;
    }
	m_soundEffects.clear();
    if (m_musicMasteringVoice != nullptr)
    {
       m_musicMasteringVoice->DestroyVoice();
    }
    if (m_soundEffectMasteringVoice != nullptr)
    {
        m_soundEffectMasteringVoice->DestroyVoice();
    }
	if(m_musicEngine != nullptr)
	{
		m_musicEngine->Release();
	}

    m_musicSourceVoice = nullptr;
    m_musicMasteringVoice = nullptr;
    m_soundEffectMasteringVoice = nullptr;
    m_soundEffectReverbVoiceSmallRoom = nullptr;
    m_soundEffectReverbVoiceLargeRoom = nullptr;
    m_musicReverbVoiceSmallRoom = nullptr;
    m_musicReverbVoiceLargeRoom = nullptr;
    m_musicEngine = nullptr;
    m_soundEffectEngine = nullptr;
}

void AudioEngine::Start()
{
	if(wcscmp(m_Background, L"") != 0)
	{
		HRESULT hr = m_musicSourceVoice->Start(0);
	   
		if SUCCEEDED(hr) 
		{
			m_isAudioStarted = true;
		}
		else
		{
			m_engineExperiencedCriticalError = true;
			m_isAudioStarted = false;
		}
	}
}

// This sample processes audio buffers during the render cycle of the application.
// As long as the sample maintains a high-enough frame rate, this approach should
// not glitch audio. In game code, it is best for audio buffers to be processed
// on a separate thread that is not synced to the main render loop of the game.
void AudioEngine::Render()
{
    if (m_engineExperiencedCriticalError)
    {
        m_engineExperiencedCriticalError = false;
        ReleaseResources();
		Initialize();
        CreateResources();	
        Start();
        if (m_engineExperiencedCriticalError)
        {
            return;
        }
}

    
    bool streamComplete;
    XAUDIO2_VOICE_STATE state;
    uint32 bufferLength;
    XAUDIO2_BUFFER buf = {0};
	HRESULT hr;

    // Use MediaStreamer to stream the buffers.
	if(m_musicSourceVoice)
	{
		m_musicSourceVoice->GetState(&state);
		while (state.BuffersQueued <= MAX_BUFFER_COUNT - 1)
		{
			streamComplete = m_musicStreamer.GetNextBuffer(
				m_audioBuffers[m_currentBuffer],
				STREAMING_BUFFER_SIZE,
				&bufferLength
				);

			if (bufferLength > 0)
			{
				buf.AudioBytes = bufferLength;
				buf.pAudioData = m_audioBuffers[m_currentBuffer];
				buf.Flags = (streamComplete) ? XAUDIO2_END_OF_STREAM : 0;
				buf.pContext = 0;

				hr = m_musicSourceVoice->SubmitSourceBuffer(&buf);
				if(FAILED(hr))
				{
					m_engineExperiencedCriticalError = true;
					return;
				}

				m_currentBuffer++;
				m_currentBuffer %= MAX_BUFFER_COUNT;
			}

			if (streamComplete)
			{
				// Loop the stream.
				m_musicStreamer.Restart();
				break;
			}

			m_musicSourceVoice->GetState(&state);
		}
	}
 
}

void AudioEngine::PlaySoundEffect(WCHAR* filename)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
    XAUDIO2_BUFFER buf = {0};
    XAUDIO2_VOICE_STATE state = {0};

	for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
    {
		if ((wcscmp(m_soundEffectsIterator->m_filename, filename) == 0))
        {
			break;
        }
    }
	if(&m_soundEffectsIterator)
	{
		if(m_soundEffectsIterator->m_soundEffectSourceVoice)
		{
			HRESULT hr = m_soundEffectsIterator->m_soundEffectSourceVoice->Start();
			if FAILED(hr)
			{
				m_engineExperiencedCriticalError = true;
				return;
			}

			// For one-off voices, submit a new buffer if there's none queued up,
			// and allow up to two collisions to be queued up.
			if (m_soundEffectsIterator->m_type != SoundEffectType::Continous)
			{
				XAUDIO2_VOICE_STATE state = {0};
				m_soundEffectsIterator->m_soundEffectSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
				if (state.BuffersQueued == 0)
				{
					m_soundEffectsIterator->m_soundEffectSourceVoice->SubmitSourceBuffer(&m_soundEffectsIterator->m_audioBuffer);
				}

			}

			m_soundEffectsIterator->m_soundEffectStarted = true;
			return;
		}
		m_soundEffectsIterator->m_soundEffectStarted = false;
	}
}

void AudioEngine::StopSoundEffect(WCHAR* filename)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
	
	HRESULT hr;
	for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
	{
		if ((wcscmp(m_soundEffectsIterator->m_filename, filename) == 0))
		{
			break;
		}
	}

	if(&m_soundEffectsIterator)
	{
		if(m_soundEffectsIterator->m_soundEffectSourceVoice)
		{
			hr = m_soundEffectsIterator->m_soundEffectSourceVoice->Stop();
			if FAILED(hr)
			{
				// If there's an error, then we'll recreate the engine on the next render pass
				m_engineExperiencedCriticalError = true;
				return;
			}
		}
	}

    m_soundEffectsIterator->m_soundEffectStarted = false;
}

bool AudioEngine::IsSoundEffectStarted(WCHAR* filename)
{
	if(m_engineExperiencedCriticalError)
	{
		return false;
	}
    for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
    {
		if ((wcscmp(m_soundEffectsIterator->m_filename, filename) == 0))
        {
			return m_soundEffectsIterator->m_soundEffectStarted;
        }
    }
	return false;
}

void AudioEngine::SetSoundEffectVolume(WCHAR* filename, float volume)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
	for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
    {
		if ((wcscmp(m_soundEffectsIterator->m_filename, filename) == 0))
        {
			m_soundEffectsIterator->m_soundEffectSourceVoice->SetVolume(volume);
        }
    }
}

void AudioEngine::SetSoundEffectPitch(WCHAR* filename, float pitch)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
	for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
    {
		if ((wcscmp(m_soundEffectsIterator->m_filename, filename) == 0))
        {
			m_soundEffectsIterator->m_soundEffectSourceVoice->SetFrequencyRatio(pitch);
        }
    }
}

void AudioEngine::SetSoundEffectFilter(WCHAR* filename, float frequency, float oneOverQ)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
	for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
    {
		if ((wcscmp(m_soundEffectsIterator->m_filename, filename) == 0))
        {
			break;
        }
    }
    if (m_soundEffectsIterator->m_soundEffectSourceVoice != nullptr) 
	{
        if (oneOverQ <= 0.1f)
        {
            oneOverQ = 0.1f;
        }
        if (oneOverQ > XAUDIO2_MAX_FILTER_ONEOVERQ)
        {
            oneOverQ = XAUDIO2_MAX_FILTER_ONEOVERQ;
        }
        XAUDIO2_FILTER_PARAMETERS params = {LowPassFilter, XAudio2CutoffFrequencyToRadians(frequency, m_soundEffectsIterator->m_soundEffectSampleRate), oneOverQ};

       m_soundEffectsIterator->m_soundEffectSourceVoice->SetFilterParameters(&params);
    }
}

// Uses the IXAudio2::StopEngine method to stop all audio immediately.
// It leaves the audio graph untouched, which preserves all effect parameters
// and effect histories (like reverb effects) voice states, pending buffers,
// cursor positions and so on.
// When the engines are restarted, the resulting audio will sound as if it had
// never been stopped except for the period of silence.
void AudioEngine::SuspendAudio()
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}

    if (m_isAudioStarted)
    {
        m_musicEngine->StopEngine();
        m_soundEffectEngine->StopEngine();
    }
    m_isAudioStarted = false;
}

// Restarts the audio streams. A call to this method must match a previous call
// to SuspendAudio. This method causes audio to continue where it left off.
// If there is a problem with the restart, the m_engineExperiencedCriticalError
// flag is set. The next call to Render will recreate all the resources and
// reset the audio pipeline.
void AudioEngine::ResumeAudio()
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}

    HRESULT hr = m_musicEngine->StartEngine();
    HRESULT hr2 = m_soundEffectEngine->StartEngine();

    if (FAILED(hr) || FAILED(hr2))
    {
        m_engineExperiencedCriticalError = true;
    }
}


void AudioEngine::SetBackgroundVolume(float volume)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
    if (m_musicSourceVoice != nullptr) 
	{
        m_musicSourceVoice->SetVolume(volume);
    }
}

void AudioEngine::SetBackgroundPitch(float pitch)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
    if (m_musicSourceVoice != nullptr) 
	{
        m_musicSourceVoice->SetFrequencyRatio(pitch);
    }
}

void AudioEngine::SetBackgroundFilter(float frequency, float oneOverQ, uint32 samplerate)
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
    if (m_musicSourceVoice != nullptr) 
	{
        if (oneOverQ <= 0.1f)
        {
            oneOverQ = 0.1f;
        }
        if (oneOverQ > XAUDIO2_MAX_FILTER_ONEOVERQ)
        {
            oneOverQ = XAUDIO2_MAX_FILTER_ONEOVERQ;
        }
        XAUDIO2_FILTER_PARAMETERS params = {LowPassFilter, XAudio2CutoffFrequencyToRadians(frequency, samplerate), oneOverQ};

        m_musicSourceVoice->SetFilterParameters(&params);
    }
}

void AudioEngine::CreateBackgroundMusic()
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}
	
	HRESULT hr;

	// Media Foundation is a convenient way to get both file I/O and format decode for
    // audio assets. You can replace the streamer in this sample with your own file I/O
    // and decode routines.

	hr = XAudio2Create(&m_musicEngine);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}




#if defined(_DEBUG)
	XAUDIO2_DEBUG_CONFIGURATION debugConfig = {0};
	debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
	debugConfig.TraceMask = XAUDIO2_LOG_ERRORS;
	m_musicEngine->SetDebugConfiguration(&debugConfig);
#endif

	m_musicEngineCallback.Initialize(this);
	m_musicEngine->RegisterForCallbacks(&m_musicEngineCallback);

	// This sample plays the equivalent of background music, which we tag on the
	// mastering voice as AudioCategory_GameMedia. In ordinary usage, if we were
	// playing the music track with no effects, we could route it entirely through
	// Media Foundation. Here we are using XAudio2 to apply a reverb effect to the
	// music, so we use Media Foundation to decode the data then we feed it through
	// the XAudio2 pipeline as a separate Mastering Voice, so that we can tag it
	// as Game Media. We default the mastering voice to 2 channels to simplify
	// the reverb logic.
		
	hr = m_musicEngine->CreateMasteringVoice(
			&m_musicMasteringVoice,
			2,
			48000,
			0,
			nullptr,
			nullptr,
			AudioCategory_GameMedia
		);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

	if(m_AudioManager && m_AudioManager->m_BackroundMusic)
	{
		m_Background = m_AudioManager->m_BackroundMusic;
	}
	else
	{
		m_engineExperiencedCriticalError = true;
		return;
	}
	if(wcscmp(m_Background, L"") != 0)
	{
		m_musicStreamer.Initialize(m_Background);
		
		CreateReverb(
			m_musicEngine,
			m_musicMasteringVoice,
			&m_reverbParametersSmall,
			&m_musicReverbVoiceSmallRoom,
			true
			);
		CreateReverb(
			m_musicEngine,
			m_musicMasteringVoice,
			&m_reverbParametersLarge,
			&m_musicReverbVoiceLargeRoom,
			true
			);

		XAUDIO2_SEND_DESCRIPTOR descriptors[3];
		descriptors[0].pOutputVoice = m_musicMasteringVoice;
		descriptors[0].Flags = 0;
		descriptors[1].pOutputVoice = m_musicReverbVoiceSmallRoom;
		descriptors[1].Flags = 0;
		descriptors[2].pOutputVoice = m_musicReverbVoiceLargeRoom;
		descriptors[2].Flags = 0;
		XAUDIO2_VOICE_SENDS sends = {0};
		sends.SendCount = 3;
		sends.pSends = descriptors;
		WAVEFORMATEX& waveFormat = m_musicStreamer.GetOutputWaveFormatEx();

		hr = m_musicEngine->CreateSourceVoice(&m_musicSourceVoice, &waveFormat, 0, 1.0f, &m_voiceContext, &sends, nullptr);
		if(FAILED(hr))
		{
			m_engineExperiencedCriticalError = true;
			return;
		}

		hr = m_musicMasteringVoice->SetVolume(0.01f);
		if(FAILED(hr))
		{
			m_engineExperiencedCriticalError = true;
			return;
		}

		m_isAudioStarted = false;
		Start();
	}
	m_engineExperiencedCriticalError = false;
}

void AudioEngine::CreateSoundEffects()
{
	if(m_engineExperiencedCriticalError)
	{
		return;
	}

	HRESULT hr;

	// Create a separate engine and mastering voice for sound effects in the sample
    // Games will use many voices in a complex graph for audio, mixing all effects down to a
    // single mastering voice.
    // We are creating an entirely new engine instance and mastering voice in order to tag
    // our sound effects with the audio category AudioCategory_GameEffects.

	hr = XAudio2Create(&m_soundEffectEngine);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}


    m_soundEffectEngineCallback.Initialize(this);
    m_soundEffectEngine->RegisterForCallbacks(&m_soundEffectEngineCallback);

    // We default the mastering voice to 2 channels to simplify the reverb logic.
	hr = m_soundEffectEngine->CreateMasteringVoice(&m_soundEffectMasteringVoice, 2, 48000, 0, nullptr, nullptr, AudioCategory_GameEffects);
	if(FAILED(hr))
	{
		m_engineExperiencedCriticalError = true;
		return;
	}

    CreateReverb(m_soundEffectEngine, m_soundEffectMasteringVoice, &m_reverbParametersSmall, &m_soundEffectReverbVoiceSmallRoom, true);
    CreateReverb(m_soundEffectEngine, m_soundEffectMasteringVoice, &m_reverbParametersLarge, &m_soundEffectReverbVoiceLargeRoom, true);

	// Create all the sound effects
	for(m_AudioManager->m_audioStoreIterator = m_AudioManager->m_audioStore.begin(); m_AudioManager->m_audioStoreIterator != m_AudioManager->m_audioStore.end(); m_AudioManager->m_audioStoreIterator++)
	{
		CreateSourceVoice(m_AudioManager->m_audioStoreIterator->m_soundName, m_AudioManager->m_audioStoreIterator->m_type);
	}

	m_engineExperiencedCriticalError = false;
}

float AudioEngine::GetBackgroundVolume()
{
	float volume;
	m_musicSourceVoice->GetVolume(&volume);
	return volume;
}

float AudioEngine::GetSoundEffectVolume(WCHAR* filename)
{
	float volume;
	for (m_soundEffectsIterator = m_soundEffects.begin(); m_soundEffectsIterator != m_soundEffects.end(); m_soundEffectsIterator++)
    {
		if ((wcscmp(m_soundEffectsIterator->m_filename, filename) == 0))
        {
			m_soundEffectsIterator->m_soundEffectSourceVoice->GetVolume(&volume);
			return volume;
        }
    }
	return 0.0f;
}
