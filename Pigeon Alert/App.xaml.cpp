//
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"

using namespace _Pigeon_Alert;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
	InitializeComponent();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
	m_Background = L"";
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used when the application is launched to open a specific file, to display
/// search results, and so forth.
/// </summary>
/// <param name="args">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs)
{
	if (pArgs->PreviousExecutionState == ApplicationExecutionState::Terminated)
	{
		//TODO: Load state from previously suspended application
	}

//<<<<<<< .mine
	m_MainPage = ref new DirectXPage(this);
	m_MainPage->Initialize();

//	// Place the frame in the current Window and ensure that it is active
	Window::Current->Content = m_MainPage;
	Window::Current->Activate();
}

/// <summary>
/// Invoked when the application is being suspended.
/// </summary>
/// <param name="sender">Details about the origin of the event.</param>
/// <param name="args">Details about the suspending event.</param>
void App::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ e)
{
	(void) sender;	// Unused parameter
	(void) e;	// Unused parameter

	//TODO: Save application state and stop any background activity
}

void App::InitializeGameState()
{

}

void App::SetGameOverlayState(GameOverlayState state)
{
	
}

void App::SetGameEngineState(GameEngineState state)
{
	
}

void App::HideGameOverlay()
{
	
}

void App::ShowGameOverlay()
{
	
}

void App::PauseGame(GameEngineState state)
{
	IsPaused = !IsPaused;
	m_MainPage->GetTimer()->Pause();
}

void App::SetAudioEngine(AudioEngine* audio)
{
	m_Audio = audio;
}

AudioEngine* App::GetAudioEngine()
{
	return m_Audio;
}

void App::SetBackgroundMusic(wchar_t* filename)
{
	wchar_t* m_filename = L"Assets/Audio/";
	int a = wcslen(filename) + wcslen(m_filename) + 1;
	wchar_t* buf = new wchar_t[a];
	wcscpy_s(buf, a, m_filename);
	wcscat_s(buf, a, filename);

	m_Background = (wchar_t *)buf;

	if(m_Audio)
	{
		m_Audio->CreateBackgroundMusic();
	}
}

void App::SetBackgroundVolume(float volume)
{
}

void App::SetBackGroundPitch(float pitch)
{
}

void App::SetBackGroundFilter(float frequency, float oneOverQ, uint32 samplerate)
{
}