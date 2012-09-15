#pragma once
#include "pch.h"
#include "Main.h"
#include "Texture.h"
#include "Layer.h"
#include "Text.h"
#include "TextureManager.h"
#include "AudioEngine.h"

class Main;
class Texture;
class Layer;
class Text;
struct TextureEntry;

enum Coordinate
{
	Local,
	Global
};

struct Sounds
{
	WCHAR*				m_filename;
	int					m_type;
};

class GameObject
{
public:
	GameObject();
	GameObject(std::wstring _textObjectName, std::wstring _FontName, float _FontSize, std::wstring _Text, Main* _Main, Vector2 &_Position);
	GameObject(std::wstring objectName, Main* _Main, bool dynamic, bool physicsBody, Vector2 &_Position);
	GameObject(std::wstring objectName, Main* _Main, bool dynamic, bool physicsBody, Vector2 &_Position, std::wstring _TextureName, bool _IsSprite, int _Rows, int _Columns);
	void SetGravity(float gravity);	
	void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_d2dContext);
	virtual void Update(unsigned long frameNumber) {}
	virtual void Start() {}
	virtual void Collided(GameObject *) {}
	virtual std::wstring GetType() { return L"GameObject"; }
	void OnPointerPressed(Vector2 _Point);
	void AddForce (float x,float y, Coordinate _RotationCoordinate);
	void OnPointerMoved(Vector2 _Point);
	void Translate(Vector2 Direction, Coordinate RotationCoordinate);
	void TranslateVelocity(Vector2 Direction, Coordinate RotationCoordinate);
	void AddForce(Vector2 _Direction, Coordinate _RotationCoordinate);	
	void RotateToLookAt(Vector2 _LookAt);
	void RotateToLookAt(float x,float y);
	void RotateToLookAtTime(Vector2 _LookAt,float change);
	void TranslateVelocity (float x,float y, Coordinate _RotationCoordinate);
	void SetDrawScale(float x,float y);
	void SetDrawScale (Vector2 Scale);
	Vector2 GetMainCameraPosition();
	void SetCollisionScale (Vector2 Scale);
	void SetScale(Vector2 &Scale);
	void RotateAroundALocalPoint(float Angle,Vector2 Point);
	Vector2 GetGlobalPositionLocalOffset(Vector2 Point);
	void SetPosition(Vector2 &Point);
	void SetCollisionScale(float x,float y);
	void Translate(float x,float y, Coordinate _RotationCoordinate);
	void SetScale(float x,float y);
	Vector2 GetVelocity();
	float GetVelocitySize();
	void SetVelocity(float x,float y);
	Vector2 GetCollisionScale();
	Vector2 GetDrawScale();
	void RotateAroundALocalPoint(float Angle, float x, float y);
	void Rotate(float angle);
	void SetRotation(float angle);
	void SetPosition(float x,float y);
	void FixRotation(bool flag);
	void SetPhysicsBody(bool flag);
	void SetDynamicBody(bool flag);
	void SetTag(std::wstring ChangeTo);
	void SetName(std::wstring ChangeTo);

	//<--- Texture Setting
	void AddTexture(std::wstring m_TextureName, bool _IsSprite, int _Rows, int _Columns);		
	void SetTexture(std::wstring m_TextureName);
	void PlaySpriteTexture();
	void PauseSpriteTexture();
	void ResumeSpriteTexture();
	void SetTextureFrame();
	void SetTextureFrame(int _FrameNumber);
	int GetCurrentTextureFrame();
	std::wstring GetTextureName();
	bool IsPlayingTexture();
	bool RotateTextureWithPhysics;
	//--->

	//<----- Font Setting 
	void SetFontColor(D2D1_COLOR_F _FontColor);
	// Setting the Size of the TextBlock
	void SetSizeOfTextBlockInPixel(float _Widht, float _Height);
	// Set Algnment of the text 
	void SetAlignmentTextBlock(int _Alignment);
	// Setting the text
	void SetText(std::wstring _Text);
	//------>

	
	bool CompareTag(std::wstring TestTo);
	bool CompareName(std::wstring TestTo);
	
	bool objectActive;
	bool IsDynamic();
	bool IsPhysicsBody();

	void AddSound(wchar_t*, int);
	void PlaySound(wchar_t*);
	void StopSound(wchar_t*);

	std::wstring GetName();
	std::wstring GetTag();
	int gameObjectId;
	Layer *  GetLayer(int layer);
	Layer *  GetMyLayer();
	void Destroy();
	void OnDestroy();
	D2D1::Matrix3x2F	m_Transform;	
	b2Body*				collisionBox;	
	int Layer;
	Vector2	scale;
	Vector2	position;
	Vector2 GetGlobalPositionLocalOffset(float x, float y);
	static Main*		p_Main;
	AudioEngine*		p_Audio;
	
protected:
	// Pointer to main class
	std::wstring			name;
	std::wstring			Tag;
	D2D1_SIZE_F		screenSize;

    int				widthScreen;
    int				heightScreen;
	
	D2D1::Matrix3x2F rot2;//=D2D1::Matrix3x2F::Identity();
	bool	dynamic;
	bool	physicsBody;
	
	float	offsetX;
	float	offsetY;
 	float	drawPositionX; 
	float	drawPositionY;
	float   x_DrawScale;
	float	y_DrawScale;
	float	x_CollisionScale;
	float	y_CollisionScale;
	float	m_OriginalWidth;
	float	m_OriginalHeight;

	float32	 angle;

	Vector2  touchPoint;

	// Store All the Textures in the list
	list<TextureEntry *> TextureHolder;
	list<TextureEntry *>::iterator m_TextureIterator;
	// Current texture using 
	Texture*				m_CurrentTexture;
	Text*					m_Text;

	list<Sounds> m_soundsList;
	list<Sounds>::iterator m_soundsListIterator;
};
