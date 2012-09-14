#pragma once
#include "Engine\Main.h"
class Main;

enum FontAlignment
{
	Left,
	Center,
	Right
};

class Text
{

	
public:
	Microsoft::WRL::ComPtr<IDWriteTextFormat>	 m_textFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_textBrush;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1>		 m_opacityBitmap;
	Microsoft::WRL::ComPtr<IDWriteTextLayout>	 m_textLayout;

	DWRITE_TEXT_METRICS m_textMetrics;

	// Pointer to main class 
	Main* p_Main;

	Text();
	Text(Main* _Main, std::wstring _FontName, float _FontSize);
	
	float MaxWidth;
	float MaxHeight;

	// Setting the Text 
	void SetText(std::wstring* _Text);
	// Setting the Font Color 
	void SetFontColor(D2D1_COLOR_F _FontColor);
	// Set Algnment of the text 
	void SetAlignment(FontAlignment _Alignment);
	// Setting the Size of the TextBlock
	void SetSizeOfTextBlockInPixel(float _Widht, float _Height);
};

