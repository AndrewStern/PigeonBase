#pragma once
#include "Main.h"
class Main;

ref class Controller
{
internal:
    Controller();

    void Initialize(_In_ Windows::UI::Core::CoreWindow^ window, Main* newMain);

protected:
   void OnPointerPressed(_In_ Windows::UI::Core::CoreWindow^ sender,_In_ Windows::UI::Core::PointerEventArgs^ args);
   void OnPointerReleased( _In_ Windows::UI::Core::CoreWindow^ sender,_In_ Windows::UI::Core::PointerEventArgs^ args);
   void OnPointerMoved( _In_ Windows::UI::Core::CoreWindow^ sender,_In_ Windows::UI::Core::PointerEventArgs^ args);
   void OnKeyDown(_In_ Windows::UI::Core::CoreWindow^ sender,_In_ Windows::UI::Core::KeyEventArgs^ args);
   void OnKeyUp( _In_ Windows::UI::Core::CoreWindow^ sender,_In_ Windows::UI::Core::KeyEventArgs^ args);

#ifdef MOVELOOKCONTROLLER_TRACE
    void DebugTrace(const wchar_t *format, ...);
#endif

private:
    void     ResetState();
    void     UpdateGameController();
	// Ref to main class
	Main*    main;
	bool     m_PointerPress;
};
//----------------------------------------------------------------------
