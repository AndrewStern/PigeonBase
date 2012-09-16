#pragma once
#include "GameObject.h"


/**
 * An enum that defines the types of sounds
 */
enum SoundEffectType
{
	Single,			/** This type of sound plays just once */
	Continous,		/** This type of sound loops infinitely until a stop sound is called */
};

/**
 * A simple class that defines a structure of a sound effect
 * It holds the name , type, buffer information etc., of the sound effect
 */
class SoundEffect
{
public:
	SoundEffectType				m_type;								/** The type of this SoundEffect */
	WCHAR*						m_filename;							/** The filename of this SoundEffect */
    IXAudio2SourceVoice*        m_soundEffectSourceVoice;			
    XAUDIO2_BUFFER              m_audioBuffer;						/** The Buffer of this SoundEffect */
    byte*                       m_soundEffectBufferData;
    uint32                      m_soundEffectBufferLength;
    uint32                      m_soundEffectSampleRate;
    bool                        m_soundEffectStarted;				/** A flag that indicates if this SoundEffect styarted playing */
};

