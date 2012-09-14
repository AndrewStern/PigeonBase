#include "pch.h"
#include "DirectXInitializer.h"
#include "UserMain.h"
#include <DirectXMath.h>

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace DirectX;

using namespace _2D_Game_Engine;

static const D2D1::ColorF sc_bgColors[] = 
{
	D2D1::ColorF::CornflowerBlue,
	D2D1::ColorF::Orchid,
	D2D1::ColorF::DeepPink,
	D2D1::ColorF::Red,
	D2D1::ColorF::DarkOrange,
	D2D1::ColorF::Gold,
	D2D1::ColorF::GreenYellow,
	D2D1::ColorF::Aqua
};

DirectXInitializer::DirectXInitializer() :
	m_animating(false),
	m_renderNeeded(true),
	m_bgColorIndex(0),
	m_viewPosition(0.0f, 0.0f)
{
}

void DirectXInitializer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			42.0f,
			L"en-US",
			&m_textFormat
			)
		);

	DX::ThrowIfFailed(
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
		);
}

void DirectXInitializer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();
	/*
	m_Audio.Initialize();
	m_Audio.CreateResources();
	((App^)Windows::UI::Xaml::Application::Current->Current)->SetAudioEngine(&m_Audio);
	m_Audio.Start();
	*/
	m_Main = new UserMain();
	m_Main->Initialize(m_d2dDevice.Get(), m_d2dContext.Get(), m_wicFactory.Get(), m_dwriteFactory.Get());
	m_Main->Start();



	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&m_blackBrush
			)
		);

	Platform::String^ text = "Hello, DirectX!";

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextLayout(
			text->Data(),
			text->Length(),
			m_textFormat.Get(),
			700, // maxWidth
			1000, // maxHeight
			&m_textLayout
			)
		);

	m_textLayout->GetMetrics(&m_textMetrics);

	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

	D2D1_SIZE_U opacityBitmapSize = D2D1::SizeU(
		static_cast<UINT32>(m_textMetrics.widthIncludingTrailingWhitespace),
		static_cast<UINT32>(m_textMetrics.height)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateBitmap(
			opacityBitmapSize,
			nullptr,
			0,
			&properties,
			&m_opacityBitmap
			)
		);

	m_d2dContext->SetTarget(m_opacityBitmap.Get());
	m_d2dContext->BeginDraw();

	//m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));

	m_d2dContext->DrawTextLayout(
		D2D1::Point2F(0.0f, 0.0f),
		m_textLayout.Get(),
		m_blackBrush.Get(),
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
		);

	DX::ThrowIfFailed(
		m_d2dContext->EndDraw()
		);

	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
	
}

void DirectXInitializer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();
}


void DirectXInitializer::Render()
{
	m_Main -> Render();
	/*if(&m_Audio)
	{
		if(!m_Audio.m_isAudioStarted)
		{
			m_Audio.Start();
		}
		m_Audio.Render();
	}*/
}

Main* DirectXInitializer::GetMain()
{
	return m_Main;
}
