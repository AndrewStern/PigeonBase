//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "Controller.h"
#include "DirectXHelper.h"

using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Windows::Devices::Input;
using namespace Windows::System;

Controller::Controller()
{
}

//----------------------------------------------------------------------
// set up the Controls supported by this controller

void Controller::Initialize(_In_ CoreWindow^ window, Main* _Main)
{
	// Set the pointer press to false in start of the game 
	m_PointerPress = false;

    window->PointerPressed +=ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Controller::OnPointerPressed);

	window->PointerMoved +=ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Controller::OnPointerMoved);

    window->PointerReleased +=ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Controller::OnPointerReleased);
	
    window->KeyDown +=ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &Controller::OnKeyDown);

    window->KeyUp +=ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &Controller::OnKeyUp);

    ResetState();

	main = _Main;
}

//----------------------------------------------------------------------

void Controller::OnPointerPressed(_In_ CoreWindow^ /* sender */,_In_ PointerEventArgs^ args)
{
	m_PointerPress = true;

	// Getting the screen size
	D2D1_SIZE_F screenSize = main->GetID2D1DeviceContext()->GetSize();
    int width  = static_cast<int>(screenSize.width);
    int height = static_cast<int>(screenSize.height);

	//x offset in meters (400/50 = 8). This will put the 0 x-coordinate in the middle of the screen horizontally, 
	//y offset in meters (300/50 = 6). This will put the 0 y-coordinate in the middle of the screen vertically.
	Vector2 OffSet = Vector2((width*0.5f)/50, (height*0.5f)/50);
	

	float x = (args->CurrentPoint->Position.X -(width*0.5f)-main->GetMainCameraPosition()._31)/50;
	float y = (-args->CurrentPoint->Position.Y +(height*0.5f)+main->GetMainCameraPosition()._32)/50;
	
	main -> OnPointerPressed(Vector2(x,y));
}

void Controller::OnPointerReleased(_In_ CoreWindow^ /* sender */, _In_ PointerEventArgs^ args)
{
	m_PointerPress = false;

	D2D1_SIZE_F screenSize = main->GetID2D1DeviceContext()->GetSize();
    int width  = static_cast<int>(screenSize.width);
    int height = static_cast<int>(screenSize.height);

	//x offset in meters (400/50 = 8). This will put the 0 x-coordinate in the middle of the screen horizontally, 
	//y offset in meters (300/50 = 6). This will put the 0 y-coordinate in the middle of the screen vertically.
	Vector2 OffSet = Vector2((width*0.5f)/50, (height*0.5f)/50);
	

	float x = (args->CurrentPoint->Position.X -(width*0.5f)-main->GetMainCameraPosition()._31)/50;
	float y = (-args->CurrentPoint->Position.Y +(height*0.5f)+main->GetMainCameraPosition()._32)/50;
	
	//main->SetTouch(false, x, y);
}

void Controller::OnPointerMoved(_In_ CoreWindow^ /* sender */,_In_ PointerEventArgs^ args)
{
	if(m_PointerPress)
	{
		// Getting the screen size
		D2D1_SIZE_F screenSize = main->GetID2D1DeviceContext()->GetSize();
		int width  = static_cast<int>(screenSize.width);
		int height = static_cast<int>(screenSize.height);

		//x offset in meters (400/50 = 8). This will put the 0 x-coordinate in the middle of the screen horizontally, 
		//y offset in meters (300/50 = 6). This will put the 0 y-coordinate in the middle of the screen vertically.
		Vector2 OffSet = Vector2((width*0.5f)/50, (height*0.5f)/50);
	

		float x = (args->CurrentPoint->Position.X -(width*0.5f)-main->GetMainCameraPosition()._31)/50;
		float y = (-args->CurrentPoint->Position.Y +(height*0.5f)+main->GetMainCameraPosition()._32)/50;
	
		main->OnPointerMoved(Vector2(x,y));
	}
}



//----------------------------------------------------------------------

void Controller::OnKeyDown( _In_ CoreWindow^ /* sender */,_In_ KeyEventArgs^ args)
{
    Windows::System::VirtualKey Key;
    Key = args->VirtualKey;
	
	main->OnKeyDown(Key);
}

//----------------------------------------------------------------------

void Controller::OnKeyUp(_In_ CoreWindow^ /* sender */, _In_ KeyEventArgs^ args)
{
    Windows::System::VirtualKey Key;
    Key = args->VirtualKey;

	main->OnKeyUp(Key);
}

//----------------------------------------------------------------------

void Controller::ResetState()
{
}

//----------------------------------------------------------------------

void Controller::UpdateGameController()
{
}


//----------------------------------------------------------------------

#ifdef Controller_TRACE
void Controller::DebugTrace(const wchar_t *format, ...)
{
    // generate the message string
    va_list args;
    va_start(args, format);
    wchar_t message[1024];
    vswprintf_s(message, 1024, format, args);
    OutputDebugStringW(message);
}
#endif

//----------------------------------------------------------------------
