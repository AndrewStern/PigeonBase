#pragma once
#include "DirectXHelper.h"
#include "GameObject.h"
#include "Controller.h"
#include "BasicTimer.h"
#include "pch.h"
#include "TextureManager.h"
#include "Layer.h"
#include "AudioManager.h"

class GameObject;
class Layer;
class TextureManager;
using namespace Microsoft::WRL;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Graphics::Display;
using namespace D2D1;
using namespace Windows::Devices::Input;
using namespace Windows::UI::Popups;


/**
 * The Main class is the mid-level game loop that handles the creation of all the game elements like the layers, gameobjects, etc.
 * The users however will NOT be using the Main class directly. Instead, they will be using the Subclass of the Main class -> the UserMain class.
 * In the UserMain, the users can add their game specific member variables and member functions. The Main class how ever is the background class that handles the creation 
 * and deletion of all the game elements based on the requirements, amongst other things like the integration of the Box2D physics engine, etc.
 */
class Main
{

public:
	/**
	 * Initializes the Main class. Sets references to the Direct2d Device, Direct2D Device Context, creates the Main camera, TextureManager, AudioManager, Physics world etc.
	 */
	void Initialize(_In_ ID2D1Device* d2dDevice, _In_ ID2D1DeviceContext*  d2dContext,_In_ IWICImagingFactory*  wicFactory,_In_ IDWriteFactory* dwriteFactory);	
	
	TextureManager * m_textureManager;
	AudioManager * m_audioManager;

	/**
	 * Renders all the existing GameObjects to the screen
	 */
	void Render();

	/**
	 * Adds a New Layer to the game and gives the current count of the layers
	 *
	 * @return		The total number of Layers in the game
	 */
	int AddLayer();

	/**
	 * Deletes a Layer from the game
	 *
	 * @param			layerId			The ID of the layer to delete from the world
	 */
	void DeleteLayer(int layerId);

	/**
	 * A virtual function to be implemented by the Main class's sub-class -> UserMain
	 * It is called by the Controller class
	 */
	virtual void OnPointerPressed(Vector2 _Point) {}

	/**
	 * A virtual function to be implemented by the Main class's sub-class -> UserMain
	 * It is called by the Controller class
	 */
	virtual void OnPointerMoved(Vector2 _Point) {}

	/**
	 * A virtual function to be implemented by the Main class's sub-class -> UserMain
	 * It is called by the Controller class
	 */
	virtual void OnPointerReleased(Vector2 _Point) {}

	/**
	 * Sets the camera 's position to the center of the specified object
	 *
	 * @param		_ObjectToLookAt			The GameObject to make the camera look at
	 */
	void SetCameraLookAt(GameObject* _ObjectToLookAT);

	/**
	 * Sets the camera 's position to the point specified
	 *
	 * @param		_Point			The point in the game world to make the camera look at
	 */
	void SetCameraLookAt(Vector2 _Point);

	/**
	 * Gets the position of the main camera as a Matrix
	 *
	 * @return		A 3x2 Matrix that represents the current position of the main camera
	 */
	D2D1::Matrix3x2F GetMainCameraPosition();

	/**
	 * Gets the position of the main camera as a Vector
	 *
	 * @return		A 3x2 Vector that represents the current position of the main camera
	 */
	Vector2 GetMainCameraPositionVector();

	/**
	 * Gets the layer with the specified ID
	 *
	 * @return		The Layer with ID =  layerId
	 */
	Layer * GetLayer(int layerId);

	/**
	 * Gets the dynamic gameobject at the specified point
	 *
	 * @param		_Point		The point to check for dynamic body
	 * @return		A GameObject at the given point if it is a dynamic body. If no dynamic GameObject is found at point, then it returns nullptr
	 */
	GameObject* GetDynamicObjectAtPoint(Vector2 _Point);

	/**
	 * Gets the gameobject at the specified point
	 *
	 * @param		_Point		The point to check for game object
	 * @return		A GameObject at the given point. If no GameObject is found at point, then it returns nullptr
	 */
	GameObject* GetObjectAtPoint(Vector2 _Point);

	/**
	 * Gets the static gameobject at the specified point
	 *
	 * @param		_Point		The point to check for static body
	 * @return		A GameObject at the given point if it is a static body. If no static GameObject is found at point, then it returns nullptr
	 */
	GameObject* GetStaticObjectAtPoint(Vector2 _Point);

	/**
	 * A flag that indicates if a touch took place
	 */
	bool touch;

	/**
	 * The point where the latest touch took place
	 */
	Vector2 touchPoint;

	/**
	 * A virtual function to be implemented by the Main class's sub-class -> UserMain
	 * It is called by the Engine on the UserMain object, when it is created
	 */
	virtual void Start(){};

	/**
	 * A virtual function to be implemented by the Main class's sub-class -> UserMain
	 * It is called by the Engine on the UserMain object each frame
	 */
	virtual void Update(unsigned long frameNumber){};

	ComPtr<ID2D1Bitmap> CreateDirectXResources(std::wstring textureName_);

	ComPtr<ID2D1DeviceContext> GetID2D1DeviceContext();

	ComPtr<IDWriteFactory> GetIDWriteFactory() {return m_dwriteFactory;}

	/**
	 * Gets the Width and Height of the screen
	 *
	 * @return		A Vector2 object containing the height and width of the screen
	 */
	Vector2 GetScreenSize(){return Vector2((float)widthScreen, (float)heightScreen);}

	/**
	 * Checks for key presses every frame.
	 * Note: Only checks for keys that its listening to. To listen to a key input, use the AddKeyListener
	 */
	void UpdateKeys();

	/**
	 * Sets the Background Music to the specified file.
	 *
	 * @param		filename		Name of the Music Track
	 */
	void SetBackgroundMusic(wchar_t* filename);

	/**
	 * Sets the volume of the Background music
	 *
	 * @param		volume			The volume to set for the background music.
	 */
	void SetBackgroundVolume(float volume);

	/**
	 * Gets the curent volumeof the background music
	 *
	 * @return		Volume of the background music
	 */
	float GetBackgroundVolume();

	/**
	 * Increments the Background volume by 1 unit
	 */
	void IncrementBackgroundVolume();

	/**
	 * Decrements the Background volume by 1 unit
	 */
	void DecrementBackgroundVolume();

	/**
	 * Converts a Directional Vector into an Angle
	 *
	 * @return		Angle of the specified Directional Vector
	 */
	float ConvertDirectionVectorToAngle(Vector2* _Direction);

	/**
	 * Converts an Angle to a Directional vector
	 *
	 * @return		Directional Vector of the specified Angle
	 */
	Vector2 ConvertDirectionAngleToVector(float _Angle);

protected:
    ComPtr<ID2D1Factory1>           m_d2dFactory;	
    ComPtr<ID2D1Device>             m_d2dDevice;	
    ComPtr<ID2D1DeviceContext>      m_d2dContext;	
    ComPtr<IDWriteFactory>          m_dwriteFactory;
    ComPtr<ID2D1SolidColorBrush>    m_whiteBrush;	
    ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;	

    ComPtr<IWICImagingFactory>      m_wicFactory;
    ComPtr<ID2D1Bitmap>             m_logoBitmap;		/** Bitmap Holding the Logo image */

    ComPtr<IDWriteTextLayout>       m_textLayout;

    UINT               m_idIncrement;
    Platform::String^  m_sampleName;

    float             m_padding;
    float             m_textVerticalOffset;
	float             m_overlayWidth;

	D2D1_SIZE_F       m_backSize;

	D2D1::Matrix3x2F  MainCamera;						/** The Main Camera of the Game */
    

	D2D1_SIZE_F		 screenSize;						/** Screen Size */
    int				 widthScreen;						/** Screen width */
    int				 heightScreen;						/** Screen Height */

	b2Body*			 body;
	b2Fixture*		 fix;

	int layerCount;										/** Total number of layers */
	Layer* m_Layers[20];								/** Layers Array */
	
	/**
	 * Adds a listener to the specified Key
	 *
	 * @param		KeyID		The Key to add a listener to.
	 * @param		Name		String to represent the key.
	 */
	void AddKeyListener(Windows::System::VirtualKey KeyID,std::string Name);

	/**
	 * Checks if the specified key is pressed
	 *
	 * @param		Name		The Key name to check against
	 * @return		True if yes, False if no
	 */
	bool KeyPressed(std::string Name);

	/**
	 * Checks if the specified key is down
	 *
	 * @param		Name		The Key name to check against
	 * @return		True if yes, False if no
	 */
	bool KeyDown(std::string Name);

	/**
	 * Checks if the specified key is released
	 *
	 * @param		Name		The Key name to check against
	 * @return		True if yes, False if no
	 */
	bool KeyRelease(std::string Name);

private:

	b2AABB _TempBody;
	unsigned long frameNumber;		/** The current frame */

	struct KeyStuct
	{
		bool ThisUpdate;
		bool LastUpdate;
		Windows::System::VirtualKey KeyID;
	};

	typedef std::map<std::string,KeyStuct> KeyMap;
	KeyMap m_Key;
	MyQueryCallback queryCallback;
};

