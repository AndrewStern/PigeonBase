#pragma once

#include <wrl.h>
#include "DirectXBase.h"
#include "BasicTimer.h"
#include "Main.h"

using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;

ref class DirectXInitializer sealed : public DirectXBase
{
public:
	DirectXInitializer();
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
internal:
	Main* GetMain();

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_opacityBitmap;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> m_textLayout;
	DWRITE_TEXT_METRICS m_textMetrics;
	Windows::Foundation::Point m_viewPosition;
	bool m_animating;
	bool m_renderNeeded;
	int m_bgColorIndex;
	Main* m_Main;
	AudioEngine m_Audio;
};