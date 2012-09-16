//
// BlankPage.xaml.h
// Declaration of the BlankPage.xaml class.
//

#pragma once
#include "pch.h"
#include "DirectXPage.g.h"
#include "Engine/DirectXInitializer.h"
#include "Engine/BasicTimer.h"
#include "Engine/Main.h"
#include "Engine/Controller.h"

using namespace Windows::UI::Xaml::Input;

namespace _Pigeon_Alert
{
	ref class Menu;
	ref class Pause;
	ref class Loading;

    [Windows::Foundation::Metadata::WebHostHidden]
	public ref class DirectXPage sealed
	{
	internal:
		DirectXPage(App^);

	public:	

		void Initialize();
		GameJam::BasicTimer^ GetTimer() { return m_timer;}
		
		Windows::UI::Xaml::Controls::Page^ GetSwapChainBackgroundPanel() { return this; };

		void SetMenuOverlay(int level, Platform::String^ objective, float timeLimit, float bonusTime);
		void SetPauseOverlay();
		void SetGameOverOverlay(bool win, int maxLevel, int highScore);
		void SetLoadingOverlay();
		void HideGameOverlay();
		void pauseButtonTapped(Object^ sender, TappedRoutedEventArgs^ args);

	private:
		~DirectXPage();
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnLogicalDpiChanged(Platform::Object^ sender);
		void OnRendering(Object^ sender, Object^ args);

		void Update();

		Windows::Foundation::EventRegistrationToken m_eventToken;

		DirectXInitializer^ m_Initializer;
		bool m_renderNeeded;

		Windows::Foundation::Point m_lastPoint;
		bool m_lastPointValid;
		
		GameJam::BasicTimer^ m_timer;

		Main* m_Main; 
		Controller^ con;

		App^ m_app;

		GameEngineState GameState;
		GameOverlayState OverlayState;

	};
}
