//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"

class AudioEngine;

namespace _Pigeon_Alert
{
	public enum class GameOverlayState
	{
		None,
		Menu,
		Pause,
		GameOver,
		Loading,
	};

	public enum class GameEngineState
	{
		Running,
		Menu,
		Pause,
		GameOver,
		Loading
	};
	
	ref class DirectXPage;

	ref class App sealed
	{
	internal:
		void SetAudioEngine(AudioEngine*);
		AudioEngine* GetAudioEngine();
		WCHAR*	m_Background;

	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs) override;

		void SetGameOverlayState(GameOverlayState);
		void SetGameEngineState(GameEngineState);

		GameOverlayState GetGameOverlayState() { return OverlayState; }
		GameEngineState GetGameEngineState() { return GameState; }
		
		void HideGameOverlay();
        void ShowGameOverlay();

		void InitializeGameState();


		App^ Get(){ return this; }
		DirectXPage^ GetMainPage(){return m_MainPage;}

		void SetBackgroundMusic(wchar_t* filename);
		void SetBackgroundVolume(float volume);
		void SetBackGroundPitch(float pitch);
		void SetBackGroundFilter(float frequency, float oneOverQ, uint32 samplerate);


	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
		void PauseGame(GameEngineState);

		bool				IsPaused;

		GameOverlayState	OverlayState;
		GameEngineState		GameState;

		DirectXPage^		m_MainPage;
		AudioEngine*		m_Audio;
	};
}
