#include "pch.h"
#include "Text.h"

// Creating the font 
Text::Text(Main* _Main, std::wstring _FontName, float _FontSize)
{
	p_Main = _Main;

	MaxWidth = 300;
	MaxHeight = 50;

	DX::ThrowIfFailed(
		p_Main->GetIDWriteFactory()->CreateTextFormat(
			_FontName.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			_FontSize,
			L"en-UK",
			&m_textFormat
			)
		);

	//By default set the Aligment to right  
	SetAlignment(Right);

	//By default set the font color to white  
	DX::ThrowIfFailed(
		p_Main->GetID2D1DeviceContext()->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&m_textBrush
			)
		);
}

// Set the Text
void Text::SetText(std::wstring* _Text)
{
	DX::ThrowIfFailed(
		p_Main->GetIDWriteFactory()->CreateTextLayout(
			_Text->c_str(),
			_Text->size(),
			m_textFormat.Get(),
			MaxWidth, // maxWidth
			MaxHeight, // maxHeight
			&m_textLayout
			)
		);

	m_textLayout->GetMetrics(&m_textMetrics);
}

// Set the Font color 
void Text::SetFontColor(D2D1_COLOR_F _FontColor)
{
	DX::ThrowIfFailed(
		p_Main->GetID2D1DeviceContext()->CreateSolidColorBrush(
			_FontColor,
			&m_textBrush
			)
		);
}

// Set Algnment of the text 
void Text::SetAlignment(FontAlignment _Alignment)
{
	if(_Alignment == Right)
	{
		DX::ThrowIfFailed(m_textFormat->SetTextAlignment( DWRITE_TEXT_ALIGNMENT_LEADING));
	}
	else if(_Alignment == Center)
	{
		DX::ThrowIfFailed(m_textFormat->SetTextAlignment( DWRITE_TEXT_ALIGNMENT_CENTER));
	}
	else if(_Alignment == Left)
	{
		DX::ThrowIfFailed(m_textFormat->SetTextAlignment( DWRITE_TEXT_ALIGNMENT_TRAILING));
	}
}

// Setting the Size of the TextBlock
void Text::SetSizeOfTextBlockInPixel(float _Widht, float _Height)
{
	MaxWidth = _Widht;
	MaxHeight = _Height;
}