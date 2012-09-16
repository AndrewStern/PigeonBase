//
// BlankPage.xaml.cpp
// Implementation of the BlankPage.xaml class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"

using namespace _2D_Game_Engine;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Input;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Graphics::Display;
using namespace concurrency;

DirectXPage::DirectXPage(App^ app) :m_renderNeeded(true),m_lastPointValid(false)
{
	InitializeComponent();

	m_app = app;

	Window::Current->CoreWindow->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);

	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);

	m_eventToken = CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &DirectXPage::OnRendering));
}

DirectXPage::~DirectXPage()
{
	 CompositionTarget::Rendering::remove(m_eventToken);
}

void DirectXPage::Initialize()
{
	m_timer = ref new GameJam::BasicTimer();

	m_app->SetGameEngineState(GameEngineState::Loading);
	m_app->ShowGameOverlay();

	m_Initializer = ref new DirectXInitializer();
	m_Initializer->Initialize(Window::Current->CoreWindow,this->DXSwapChainPanel,DisplayProperties::LogicalDpi);
	m_Main = m_Initializer->GetMain();
	con = ref new Controller();
	con -> Initialize(Windows::UI::Core::CoreWindow::GetForCurrentThread(), m_Main);

}

void DirectXPage::OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args)
{
	auto currentPoint = args->GetCurrentPoint(nullptr);
	if (currentPoint->IsInContact)
	{
		if (m_lastPointValid)
		{
			Windows::Foundation::Point delta(
				currentPoint->Position.X - m_lastPoint.X,
				currentPoint->Position.Y - m_lastPoint.Y
				);
			m_renderNeeded = true;
		}
		m_lastPoint = currentPoint->Position;
		m_lastPointValid = true;
	}
	else
	{
		m_lastPointValid = false;
	}
}

void DirectXPage::OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args)
{
	m_lastPointValid = false;
}

void DirectXPage::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_Initializer->UpdateForWindowSizeChange();
	m_renderNeeded = true;
}

void DirectXPage::OnLogicalDpiChanged(Platform::Object^ sender)
{
	m_Initializer->SetDpi(DisplayProperties::LogicalDpi);
	m_renderNeeded = true;
}

void DirectXPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
{
	m_timer->Update();
	m_Initializer->Render();
	m_Initializer->Present();
}
 
