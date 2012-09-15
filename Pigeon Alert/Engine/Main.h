#pragma once
#include "DirectXHelper.h"
#include "GameObject.h"
#include "Controller.h"
#include "BasicTimer.h"
#include "pch.h"
#include "TextureManager.h"
#include "Layer.h"

class GameObject;
class Layer;
class TextureManager;
using namespace Microsoft::WRL;

class Main
{

public:
	void Initialize(_In_ ID2D1Device* d2dDevice, _In_ ID2D1DeviceContext*  d2dContext,_In_ IWICImagingFactory*  wicFactory,_In_ IDWriteFactory* dwriteFactory);	
	TextureManager * m_textureManager;
	void Render();
	int AddLayer();
	void DeleteLayer(int layerId);
	virtual void OnPointerPressed(Vector2 _Point) {}
	virtual void OnPointerMoved(Vector2 _Point) {}
	virtual void OnPointerReleased(Vector2 _Point) {}
	void SetCameraLookAt(GameObject* _ObjectToLookAT);
	D2D1::Matrix3x2F GetMainCameraPosition();
	Layer * GetLayer(int layerId);
	virtual void OnKeyDown(Windows::System::VirtualKey Key) {}
	virtual void OnKeyUp(Windows::System::VirtualKey Key) {}
	GameObject* GetDynamicObjectAtPoint(Vector2 _Point);
	GameObject* GetObjectAtPoint(Vector2 _Point);
	GameObject* GetStaticObjectAtPoint(Vector2 _Point);
	bool touch;
	Vector2 touchPoint;
	virtual void Start(){};
	virtual void Update(unsigned long frameNumber){};
	ComPtr<ID2D1Bitmap> CreateDirectXResources(std::wstring textureName_);

	ComPtr<ID2D1DeviceContext> GetID2D1DeviceContext();
	ComPtr<IDWriteFactory> GetIDWriteFactory() {return m_dwriteFactory;}
	Vector2 GetScreenSize(){return Vector2((float)widthScreen, (float)heightScreen);}

	void SetBackgroundMusic(wchar_t* filename);

protected:
    ComPtr<ID2D1Factory1>           m_d2dFactory;
    ComPtr<ID2D1Device>             m_d2dDevice;
    ComPtr<ID2D1DeviceContext>      m_d2dContext;
    ComPtr<IDWriteFactory>          m_dwriteFactory;
    ComPtr<ID2D1SolidColorBrush>    m_whiteBrush;
    ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;

    ComPtr<IWICImagingFactory>      m_wicFactory;
    ComPtr<ID2D1Bitmap>             m_logoBitmap;

    ComPtr<IDWriteTextLayout>       m_textLayout;

    UINT               m_idIncrement;
    Platform::String^  m_sampleName;

    float             m_padding;
    float             m_textVerticalOffset;
	float             m_overlayWidth;

	D2D1_SIZE_F       m_backSize;

	D2D1::Matrix3x2F  MainCamera;
    

	D2D1_SIZE_F		 screenSize;
    int				 widthScreen;
    int				 heightScreen;

	b2Body*			 body;
	b2Fixture*		 fix;

	int layerCount;
	Layer* m_Layers[20];

private:
	b2AABB _TempBody;
	unsigned long frameNumber;
	MyQueryCallback queryCallback;
};

