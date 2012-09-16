#include "pch.h"
#include "Main.h"
#include "GameContactListener.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Graphics::Display;
using namespace D2D1;
using namespace Windows::Devices::Input;

using namespace _Pigeon_Alert;

class b2World;

// Initialize Box2D Physics 
b2World* world;
b2Fixture* fix;
b2Vec2 gravity(0.0f, -10.0f);
b2World myWorld(gravity);

void Main::Initialize(_In_ ID2D1Device* d2dDevice, _In_ ID2D1DeviceContext*  d2dContext,_In_ IWICImagingFactory*  wicFactory, _In_ IDWriteFactory* dwriteFactory)
{
	MainCamera =D2D1::Matrix3x2F::Identity();
	m_textureManager = new TextureManager();
	layerCount = 0;
	frameNumber = 0;
	// <-------------------- Initializing directx device
	m_wicFactory = wicFactory;
    m_dwriteFactory = dwriteFactory;
    m_d2dDevice = d2dDevice;
    m_d2dContext = d2dContext;
    m_padding = 3.0f;
    m_textVerticalOffset = 5.0f;
	m_backSize = D2D1::SizeF(0.0f, 0.0f);
    m_overlayWidth = 0.0f;

    ComPtr<ID2D1Factory> factory;
    d2dDevice->GetFactory(&factory);

    DX::ThrowIfFailed( factory.As(&m_d2dFactory));
	// -------------------------->
	touch = false;
		 
	// Initializing Box2D 
	world = &myWorld;
	
	GameContactListener* gcl = new GameContactListener();
	world->SetContactListener(gcl);
}
Layer * Main::GetLayer(int layerId)
{
	if (layerId<layerCount)
	{
		return m_Layers[layerId];
	}
	else
	{
		return NULL;
	}
}
void Main::DeleteLayer(int layerId)
{
	if (layerId<layerCount)
	{
		m_Layers[layerId]->Destroy();
	}
}

int Main::AddLayer()
{
	Layer * layer = new Layer(layerCount);
	m_Layers[layerCount] = layer;
	layerCount++;
	return layerCount-1;
}

Microsoft::WRL::ComPtr<ID2D1Bitmap>  Main::CreateDirectXResources(std::wstring textureName_)
{
	double dpiX = 96.0f;
    double dpiY = 96.0f;
	
	ComPtr<IWICBitmapDecoder> wicBitmapDecoder;
    DX::ThrowIfFailed(
        m_wicFactory->CreateDecoderFromFilename(
			textureName_.c_str(), 
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            &wicBitmapDecoder
            )
        );

    ComPtr<IWICBitmapFrameDecode> wicBitmapFrame;
    DX::ThrowIfFailed(
        wicBitmapDecoder->GetFrame(0, &wicBitmapFrame)
        );

    ComPtr<IWICFormatConverter> wicFormatConverter;
    DX::ThrowIfFailed(
        m_wicFactory->CreateFormatConverter(&wicFormatConverter)
        );

    DX::ThrowIfFailed(
        wicFormatConverter->Initialize(
            wicBitmapFrame.Get(),
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeCustom  // the BGRA format has no palette so this value is ignored
            )
        );

	Microsoft::WRL::ComPtr<ID2D1Bitmap> m_backBitmap;

	DX::ThrowIfFailed(
        m_d2dContext->CreateBitmapFromWicBitmap(
            wicFormatConverter.Get(),
            BitmapProperties(
                PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
                static_cast<float>(dpiX),
                static_cast<float>(dpiY)
                ),
            &m_backBitmap
            )
        );


	 DX::ThrowIfFailed( m_d2dFactory->CreateDrawingStateBlock(&m_stateBlock));

	 return m_backBitmap;

}

void Main::Render()
{

	#pragma region Getting Screen Infromation
		screenSize = m_d2dContext->GetSize();
		widthScreen = static_cast<int>(screenSize.width);
		heightScreen = static_cast<int>(screenSize.height);
	#pragma endregion

	Update(++frameNumber);

	// Step the physics world
	world->Step(1.0f/60.0f, 6, 2);

	// This is for Texture 
	if (m_stateBlock!=NULL)
    {
        m_d2dContext->SaveDrawingState(m_stateBlock.Get());
	}
	// Start drawing 
    m_d2dContext->BeginDraw();
	// Set the world Transform
    m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());
	// Clear the buffer before adding new objects 
	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	//Update Loop
	//Fire the update function on all gameobjects
	for(int i=0; i<layerCount; i++)
	{
		if (m_Layers[i]!=nullptr)
		{
			m_Layers[i]->Update(frameNumber);
		}
	}

	//Render Loop
	//This will loop through all the layers and update them
	for(int i=0; i<layerCount; i++)
	{
		if (m_Layers[i]!=nullptr)
		{
			m_Layers[i]->Render(m_d2dContext);
		}
	}

	//Cull Loop
	//This loop will remove any objects which are flagged for deletion
	for(int i=0; i<layerCount; i++)
	{
		if (m_Layers[i]!=nullptr)
		{
			m_Layers[i]->AddRemoveObjects();
			if (m_Layers[i]->layerDeleted)
			{
				delete(m_Layers[i]);
				for(int x=i+1; i<layerCount; i++)
				{
					m_Layers[x-1] = m_Layers[x];
				}
				layerCount--;
				m_Layers[layerCount] = NULL;
			}
		}
	}

    m_d2dContext->EndDraw();

	if (m_stateBlock!=NULL)
    {
		m_d2dContext->RestoreDrawingState(m_stateBlock.Get());
	}
}

ComPtr<ID2D1DeviceContext> Main::GetID2D1DeviceContext()
{
	return m_d2dContext;
}

// Set the Main Camera to look at game object
void Main::SetCameraLookAt(GameObject* _ObjectToLookAT)
{
	// Set the camera position to the Object Position
	MainCamera = _ObjectToLookAT->m_Transform*D2D1::Matrix3x2F::Translation(-widthScreen*0.5f,-heightScreen*0.5f);

	// Invert the camera to keep up with moving object 
	MainCamera.Invert();
}


D2D1::Matrix3x2F Main::GetMainCameraPosition()
{
	return MainCamera;
}


GameObject* Main::GetDynamicObjectAtPoint(Vector2 _Point)
{
	// Make a small box where the point is
	GameObject *bob = nullptr;

	_TempBody.lowerBound.Set(_Point.x - 0.001f, _Point.y - 0.001f);
	_TempBody.upperBound.Set(_Point.x + 0.001f, _Point.y + 0.001f);

	myWorld.QueryAABB(&queryCallback, _TempBody);

	
  for (unsigned int i = 0; i < queryCallback.foundBodies.size(); i++) 
  {
	  if(queryCallback.foundBodies[i]->GetUserData() != nullptr)
	  {
		 bob = (GameObject*)queryCallback.foundBodies[i]->GetUserData();
		 if(bob->IsDynamic())
		 {
			queryCallback.foundBodies.clear();
			 return bob;
		 }
	  }
	
  }
  return nullptr;
}

GameObject* Main::GetStaticObjectAtPoint(Vector2 _Point)
{
	// Make a small box where the point is
	GameObject *bob = nullptr;

	_TempBody.lowerBound.Set(_Point.x - 0.001f, _Point.y - 0.001f);
	_TempBody.upperBound.Set(_Point.x + 0.001f, _Point.y + 0.001f);

	myWorld.QueryAABB(&queryCallback, _TempBody);

	
  for (unsigned int i = 0; i < queryCallback.foundBodies.size(); i++) 
  {
	  if(queryCallback.foundBodies[i]->GetUserData() != nullptr)
	  {
		 bob = (GameObject*)queryCallback.foundBodies[i]->GetUserData();
		 if(!bob->IsDynamic())
		 {
			 queryCallback.foundBodies.clear();
			 return bob;
		 }
	  }
	
  }
  return nullptr;
}

GameObject* Main::GetObjectAtPoint(Vector2 _Point)
{
	// Make a small box where the point is
	GameObject *bob = nullptr;

	_TempBody.lowerBound.Set(_Point.x - 0.001f, _Point.y - 0.001f);
	_TempBody.upperBound.Set(_Point.x + 0.001f, _Point.y + 0.001f);

	myWorld.QueryAABB(&queryCallback, _TempBody);

	
  for (unsigned int i = 0; i < queryCallback.foundBodies.size(); i++) 
  {
	  if(queryCallback.foundBodies[i]->GetUserData() != nullptr)
	  {
			bob = (GameObject*)queryCallback.foundBodies[i]->GetUserData();
			queryCallback.foundBodies.clear();
			return bob;
	  }
	
  }
  return nullptr;
}

void Main::SetBackgroundMusic(wchar_t* filename)
{
	((App^)Application::Current->Current)->SetBackgroundMusic(filename);
}