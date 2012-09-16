#pragma once
#include <string>

class AudioEngine;

/**
 * Holds information of a sound effect
 */
struct AudioEntry
{
	int					m_refCount;		/** number of times a particular sound was added */
	WCHAR*				m_soundName;	/** name of the sound */
	int					m_type;			/** type of the sound */
};


/**
 * Holds information of a background track
 */
struct MusicEntry
{
	int					m_refCount;		/** number of times a track was added */
	WCHAR*				m_soundName;	/** name of the sound track */
};


/**
 * Audio manager class keeps track of all the sounds currently relevant to the game and plays/stops 
 */
class AudioManager
{
	public:
		/**
		 * Adds a sound file to the m_audioStore list if it does not already exist
		 *
		 * @param		_SoundName		The name of the sound to add to the list
		 * @param		_Type			The type of the sound effect
		 */
		AudioEntry * AddSound(wchar_t* _SoundName, int _Type);

		/**
		 * The list of all the audio files in the game
		 */
		std::list <AudioEntry> m_audioStore;

		/**
		 * Iterator to traverse the m_audioStore list
		 */
		std::list <AudioEntry>::iterator m_audioStoreIterator;

		/**
		 * Adds the Background track to the list
		 * Note: currently only sets the latest track added as the background. Does NOT keep track of the older files
		 *
		 * @param		_musicName		Name of the music track
		 */
		void AddMusic(wchar_t* _MusicName);

		/**
		 * The Current Background track
		 */
		wchar_t* m_BackroundMusic;

		/**
		 * Deletes/Removes the sound file from the m_audioStore list
		 *
		 * @param		_SoundName		Name of the sound to add to the list
		 */
		void DeleteSound(wchar_t* _SoundName);

		/**
		 * Sets a reference to the audio Engine
		 *
		 *@param		audio		Reference to the audio engine
		 */
		void Initialize(AudioEngine* audio);

		/**
		 * Plays the specified sound
		 *
		 * @param		soundName		Name of the sound to play
		 */
		void PlaySound(wchar_t* soundName);

		/**
		 * Stops playing the specified sound
		 *
		 * @param		soundName		Name of the soud to stop
		 */
		void StopSound(wchar_t* soundName);

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
		void SetBackGroundPitch(float pitch);

		/**
		 * Sets the filter to the specified sound effect
		 *
		 * @param		frequency			The frequency value to apply to the sound effect.
		 * @param		oneOverQ	
		 * @param		samplerate			The samplerate to apply to the Background music
		 */
		void SetBackGroundFilter(float frequency, float oneOverQ, uint32 samplerate);

		/**
		 * A reference to the AudioEngine
		 */
		AudioEngine* p_Audio;
};