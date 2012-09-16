#include "pch.h"
#include "Main.h"
#include <assert.h>
#include <windows.h>
#include <sstream>
#include "GameContactListener.h"


using namespace Windows::UI::Xaml;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Graphics::Display;
using namespace D2D1;
using namespace Windows::Devices::Input;
using namespace Windows::UI::Popups;

using namespace _2D_Game_Engine;

class b2World;

// Initialize Box2D Physics 
b2World* world;
b2Fixture* fix;
b2Vec2 gravity(0.0f, -10.0f);
b2World myWorld(gravity);

extern float meterToPixel;

void Main::Initialize(_In_ ID2D1Device* d2dDevice, _In_ ID2D1DeviceContext*  d2dContext,_In_ IWICImagingFactory*  wicFactory, _In_ IDWriteFactory* dwriteFactory)
{

	MainCamera =D2D1::Matrix3x2F::Identity();
	m_textureManager = new TextureManager();
	m_audioManager = new AudioManager();
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
   
	HRESULT hr = m_wicFactory->CreateDecoderFromFilename(
			textureName_.c_str(), 
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            &wicBitmapDecoder
            );

    if(FAILED(hr))
	{
		#if defined(_DEBUG)	
			assert(false && "Image File Not Found! Make Sure Its In Your Assests Folder");       
			return nullptr;
		#endif
	}



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

	Update(++frameNumber);
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

void Main::SetCameraLookAt(Vector2 _Point)
{
 screenSize = m_d2dContext->GetSize();
    widthScreen = static_cast<int>(screenSize.width);
    heightScreen = static_cast<int>(screenSize.height);

 float offsetX = (widthScreen*0.5f)/meterToPixel; //x offset in meters (400/50 = 8). This will put the 0 x-coordinate in the middle of the screen horizontally.
 float offsetY = (heightScreen*0.5f)/meterToPixel; //y offset in meters (300/50 = 6). This will put the 0 y-coordinate in the middle of the screen vertically.
 float drawPositionX = (_Point.x + offsetX) * meterToPixel; //( (0m) +  8.0m )* 50 = 400 pixels
 float drawPositionY = (-_Point.y + offsetY) * meterToPixel; //( -(4m) + 6.0m ) * 50 = 100 pixels
 D2D1::Matrix3x2F m_Transform = D2D1::Matrix3x2F::Translation(drawPositionX,drawPositionY);


 //D2D1::Matrix3x2F _PosMatrix = D2D1::Matrix3x2F(1,0,0,1,drawPositionX,drawPositionY);
 // Set the camera position to the Object Position
 MainCamera = m_Transform*D2D1::Matrix3x2F::Translation(-widthScreen*0.5f,-heightScreen*0.5f);

 // Invert the camera to keep up with moving object 
 MainCamera.Invert();
}


D2D1::Matrix3x2F Main::GetMainCameraPosition()
{
	return MainCamera;
}

Vector2 Main::GetMainCameraPositionVector()
{
	return Vector2(-MainCamera._31/50,MainCamera._32/50);
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

// Background Music Methods
void Main::SetBackgroundMusic(wchar_t* filename)
{
	m_audioManager->AddMusic(filename);
}


void Main::SetBackgroundVolume(float newVolume)
{
	m_audioManager->p_Audio->SetBackgroundVolume(newVolume);
}
float Main::GetBackgroundVolume()
{
	return m_audioManager->p_Audio->GetBackgroundVolume();
}
void Main::IncrementBackgroundVolume()
{
	SetBackgroundVolume(GetBackgroundVolume()+0.1f);
}
void Main::DecrementBackgroundVolume()
{
	SetBackgroundVolume(GetBackgroundVolume()-0.1f);
}

bool Main::KeyPressed(std::string Name)
{
	auto it = m_Key.find(Name);
	if (it == m_Key.end())
    {
        return false;
    }
	if(m_Key[Name].ThisUpdate && !m_Key[Name].LastUpdate)
	{
	return true;
	}

	return false;
}

bool Main::KeyDown(std::string Name)
{
	auto it = m_Key.find(Name);
	if (it == m_Key.end())
    {
        return false;
    }
	return m_Key[Name].ThisUpdate;
}

bool Main::KeyRelease(std::string Name)
{
	auto it = m_Key.find(Name);
	if (it == m_Key.end())
    {
        return false;
    }
	if(!m_Key[Name].ThisUpdate && m_Key[Name].LastUpdate)
	{
	return true;
	}
	return false;
}

void Main::AddKeyListener(Windows::System::VirtualKey KeyID,std::string Name)
{
	KeyStuct node;
    node.KeyID =KeyID ;
    node.LastUpdate=false;
	node.LastUpdate=false;
	m_Key.insert(pair<std::string,KeyStuct>(Name,node));
}

void Main::UpdateKeys()
{

	

  // std::wstring ws1(Window::Current->CoreWindow->GetAsyncKeyState(Windows::System::VirtualKey::S).ToString()->Data() ); 
  // OutputDebugStringW(ws1.c_str());
  // 	OutputDebugStringW(L"\n");
	/*if(Window::Current->CoreWindow->GetAsyncKeyState(Windows::System::VirtualKey::S)==Windows::UI::Core::CoreVirtualKeyStates::Down)
		OutputDebugStringW(L"A down \n");
	else
			OutputDebugStringW(L"A up \n");*/
		
	typedef std::map<std::string,KeyStuct>::iterator it_type;
	for(it_type iterate = m_Key.begin(); iterate != m_Key.end(); iterate++) 
	{
		iterate->second.LastUpdate=iterate->second.ThisUpdate;
		if(Window::Current->CoreWindow->GetAsyncKeyState(iterate->second.KeyID)==Windows::UI::Core::CoreVirtualKeyStates::Down)
		{
			iterate->second.ThisUpdate=true;
			//OutputDebugStringW(Windows::UI::Core::CoreVirtualKeyStates::Down.ToString());
		}
		else if(Window::Current->CoreWindow->GetAsyncKeyState(iterate->second.KeyID)==Windows::UI::Core::CoreVirtualKeyStates::None)
		{
			//OutputDebugStringW(L"A up \n");
			iterate->second.ThisUpdate=false;
		}
	}

}

// Convert directional vector to angel
float Main::ConvertDirectionVectorToAngle(Vector2* _Direction)
{
	
	float _Angle = atan2(_Direction->x,_Direction->y);
	// 1 radian = 57.2957795 degrees
	_Angle *=  57.2957795f;

	if(_Angle<0)
	{
		_Angle = 360 + _Angle;
	}
	return (_Angle);

}

// Convert angle to a directional vector
Vector2 Main::ConvertDirectionAngleToVector(float _Angle)
{
	// 1 degree = 0.0174532925 radians
	Vector2* tempPos =  new Vector2(sin(_Angle*0.0174532925f),cos(_Angle*0.0174532925f));
	return (*tempPos);
}