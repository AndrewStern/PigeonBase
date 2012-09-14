#include "pch.h"
#include "GameObject.h"

using namespace _2D_Game_Engine;

extern b2World* world; 
float rot=0;
float meterToPixel = 50.0; //50 pixels to a meter

GameObject::GameObject() :
	objectActive(true),
	gameObjectId(0),
	RotateTextureWithPhysics(true)
{
	name = L"Object";
	p_Audio = ((App^)Windows::UI::Xaml::Application::Current->Current)->GetAudioEngine();
}

// Text GameObject
GameObject::GameObject(std::wstring _textObjectName, std::wstring _FontName, float _FontSize, std::wstring _Text, Main* _Main, Vector2 &_Position)
:	objectActive(true),
	gameObjectId(0),
	RotateTextureWithPhysics(true)
{
	// Setting the gameobject name 
	name = _textObjectName;

	// Ref to main Class 
	p_Main = _Main;
	p_Audio = ((App^)Windows::UI::Xaml::Application::Current->Current)->GetAudioEngine();

	x_DrawScale		 = 1;
	y_DrawScale		 = 1;
	x_CollisionScale = 1;
	y_CollisionScale = 1;

	// We are not using any textures so set this to null
	m_CurrentTexture = NULL;

	// Setting the Text gameobjects
	m_Text = new Text(_Main, _FontName, _FontSize);
	SetText(_Text);
	
	
	// Setting the game object Position
	position = _Position;

	m_Transform=D2D1::Matrix3x2F::Identity();
	rot2=D2D1::Matrix3x2F::Identity();

	// By default the text gameobejct is not a dynamic so it won't react with gravity etc... 
	dynamic = false;
	physicsBody = false;

	b2BodyDef BodyDef;
	b2PolygonShape Box;			
	BodyDef.position.Set(position.x, position.y);	

	if(dynamic)
	{
		BodyDef.type = b2_dynamicBody;
	}

	b2Body* body;
	body = world->CreateBody(&BodyDef);
	body->SetUserData((void*)this);
	Box.SetAsBox((m_Text->MaxWidth*0.5f)/50,(m_Text->MaxHeight*0.5f)/50);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &Box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.8f;

	body->CreateFixture(&fixtureDef);
	collisionBox = body;

	if(!physicsBody)
	{
		collisionBox->SetActive(false);
	}

	// Setting the original Height and widht
	m_OriginalWidth = m_Text->MaxWidth;
	m_OriginalHeight = m_Text->MaxHeight;
}

// Empty GameObject
Main * GameObject::p_Main;

GameObject::GameObject(std::wstring objectName, Main* _Main, bool isDynamic, bool isPhysicsBody, Vector2 &_Position)
:	objectActive(true),
	gameObjectId(0),
	RotateTextureWithPhysics(true)
{
	// setting the gameobject name 
	name = objectName;

	x_DrawScale=1;
	y_DrawScale=1;
	x_CollisionScale=1;
	y_CollisionScale=1;

	// set the current texture to null
	m_CurrentTexture = NULL;
	// set the text to null 
	m_Text = NULL;

	// Ref to main Class 
	GameObject::p_Main = _Main;
	p_Audio = ((App^)Windows::UI::Xaml::Application::Current->Current)->GetAudioEngine();
	
	// Setting the game object Position
	position = _Position;

	x_CollisionScale=1;
	y_CollisionScale=1; 
	x_DrawScale=1;
	y_DrawScale=1;

	m_Transform=D2D1::Matrix3x2F::Identity();
	rot2=D2D1::Matrix3x2F::Identity();

	dynamic = isDynamic;
	physicsBody = isPhysicsBody;
	b2BodyDef BodyDef;
	b2PolygonShape Box;			
	BodyDef.position.Set(position.x, position.y);	

	if(dynamic)
	{
		BodyDef.type = b2_dynamicBody;
	}

	b2Body* body;
	body = world->CreateBody(&BodyDef);
	body->SetUserData((void*)this);
	Box.SetAsBox(1,1);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &Box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.8f;

	body->CreateFixture(&fixtureDef);
	collisionBox = body;

	if(!physicsBody)
	{
		collisionBox->SetActive(false);
	}


	// Setting the original Height and widht
	m_OriginalWidth = 1;
	m_OriginalHeight = 1;
}

// Seting the Velocity of the gameobject
void GameObject::SetVelocity(float x,float y)
{
	collisionBox->SetLinearVelocity(b2Vec2(x,y));
}

// Full GameObject with texture
GameObject::GameObject(std::wstring objectName, Main* _Main, bool isDynamic, bool isPhysicsBody, Vector2 &_Position, std::wstring _TextureName, bool _IsSprite, int _Rows, int _Columns)
:	objectActive(true),
	gameObjectId(0),
	RotateTextureWithPhysics(true)
{	
	gameObjectId = 0;
	x_DrawScale=1;
	y_DrawScale=1;
	x_CollisionScale=1;
	y_CollisionScale=1;

	// Ref to main Class 
	GameObject::p_Main = _Main;
	p_Audio = ((App^)Windows::UI::Xaml::Application::Current->Current)->GetAudioEngine();

	// Setting the gameobject name 
	name=objectName;

	// Setting up the textur class
	AddTexture(_TextureName, _IsSprite, _Rows, _Columns);
	m_CurrentTexture = TextureHolder.front()->texture;
	// Set the text to null
	m_Text = NULL;

	// Setting the game object Position
	position.x = _Position.x;
	position.y = _Position.y;

	m_Transform=D2D1::Matrix3x2F::Identity();
	rot2=D2D1::Matrix3x2F::Identity();

	dynamic = isDynamic;
	physicsBody = isPhysicsBody;
	b2BodyDef BodyDef;
	b2PolygonShape Box;			
	BodyDef.position.Set(position.x, position.y);	

	if(dynamic)
	{
		BodyDef.type = b2_dynamicBody;
	}

	b2Body* body;
	body = world->CreateBody(&BodyDef);
	body->SetUserData((void*)this);
	Box.SetAsBox((m_CurrentTexture->width*0.5f)/50,( m_CurrentTexture->height*0.5f)/50);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &Box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.8f;
	body->CreateFixture(&fixtureDef);
	collisionBox = body;

	if(!physicsBody)
	{
		collisionBox->SetActive(false);
	}


	// Setting the original Height and widht
	m_OriginalWidth = m_CurrentTexture->width;
	m_OriginalHeight = m_CurrentTexture->height;
}

void GameObject::SetTag(std::wstring ChangeTo)
{
	Tag=ChangeTo;
}
void GameObject::SetName(std::wstring ChangeTo)
{
	name=ChangeTo;
}

void GameObject::SetGravity(float gravity)
{
	collisionBox->SetGravityScale(gravity);
}
std::wstring GameObject::GetName()
{
	return name;
}
std::wstring GameObject::GetTag()
{
	return Tag;
}
bool GameObject::CompareTag(std::wstring TestTo)
{
	if(TestTo.compare(Tag)==0)
	{
		return true;
	}
	
	return false;
}

bool GameObject::CompareName(std::wstring TestTo)
{
	if(TestTo.compare(name)==0)
	{
		return true;
	}
	return false;
}

void GameObject::AddTexture(std::wstring _TextureName, bool _IsSprite, int _Rows, int _Columns)
{
	TextureHolder.push_back(GameObject::p_Main->m_textureManager->AddTexture(GameObject::p_Main, _TextureName, _IsSprite, _Rows, _Columns));	
}

void GameObject::SetTexture(std::wstring _TextureName)
{
	for(m_TextureIterator=TextureHolder.begin(); m_TextureIterator!=TextureHolder.end(); m_TextureIterator++)
	{	
		TextureEntry * m_tex = *m_TextureIterator;
		if(m_tex->texture->m_TextureName.compare(_TextureName)==0)
		{			
			m_CurrentTexture = m_tex->texture;
			// Set the texture frame to 0 
			SetTextureFrame();
			return;
		}
	}
}

void GameObject::SetPhysicsBody(bool flag)
{ 
	collisionBox->SetActive(flag);
}

void GameObject::SetDynamicBody(bool flag)
{ 		 
	if(flag)
	{
		collisionBox->SetType(b2BodyType::b2_dynamicBody);
	}
	else
	{
		collisionBox->SetType(b2BodyType::b2_staticBody);
	}
}
Layer * GameObject::GetLayer(int layer)
{
	return GameObject::p_Main->GetLayer(layer);
}
Layer * GameObject::GetMyLayer()
{
	return GetLayer(Layer);
}
static bool deleteAll(TextureEntry * theElement )
{
	//We're no longer cleaning up textures because they are handled sanely by
	//the TextureManager. We should probably add reference counting to the 
	//TextureManager and release when we are no longer using a particular texture,
	//but for now it's no big deal to keep them in memory.
	//theElement->m_Bitmap.ReleaseAndGetAddressOf();
	//delete theElement; 
	GameObject::p_Main->m_textureManager->DeleteTexture(theElement->textureName);
	return true; 
}
void GameObject::OnDestroy()
{
	TextureHolder.remove_if(deleteAll);
	world->DestroyBody(collisionBox);
}
void GameObject::Destroy()
{
	GetMyLayer()->RemoveObject(name);
}

void GameObject::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_d2dContext)
{  
	if (!objectActive) return; //Easy way to prevent this gameobject from being rendered

	screenSize = m_d2dContext->GetSize();
    widthScreen = static_cast<int>(screenSize.width);
    heightScreen = static_cast<int>(screenSize.height);

	offsetX = (widthScreen*0.5f)/meterToPixel; //x offset in meters (400/50 = 8). This will put the 0 x-coordinate in the middle of the screen horizontally.
	offsetY = (heightScreen*0.5f)/meterToPixel; //y offset in meters (300/50 = 6). This will put the 0 y-coordinate in the middle of the screen vertically.
	drawPositionX = (position.x + offsetX) * meterToPixel; //( (0m) +  8.0m )* 50 = 400 pixels
	drawPositionY = (-position.y + offsetY) * meterToPixel; //( -(4m) + 6.0m ) * 50 = 100 pixels
	m_Transform = D2D1::Matrix3x2F::Translation(drawPositionX,drawPositionY);
	
	rot=collisionBox->GetAngle();
	rot=-rot*(180/3.141592f);
	rot2=D2D1::Matrix3x2F::Rotation(rot,D2D1::Point2F((m_OriginalWidth*0.5f),( m_OriginalHeight*0.5f)));

	if(RotateTextureWithPhysics)
	{
		m_d2dContext->SetTransform(rot2*D2D1::Matrix3x2F::Scale(x_DrawScale,y_DrawScale,D2D1::Point2F((m_OriginalWidth*0.5f),( m_OriginalHeight*0.5f)))*(m_Transform*GameObject::p_Main->GetMainCameraPosition()*D2D1::Matrix3x2F::Translation(-m_OriginalWidth*0.5f,-m_OriginalHeight*0.5f)));
	
	}
	else
	{
		// No Texture rotation with physics 
		m_d2dContext->SetTransform(D2D1::Matrix3x2F::Scale(x_DrawScale,y_DrawScale,D2D1::Point2F((m_OriginalWidth*0.5f),( m_OriginalHeight*0.5f)))*(m_Transform*GameObject::p_Main->GetMainCameraPosition()*D2D1::Matrix3x2F::Translation(-m_OriginalWidth*0.5f,-m_OriginalHeight*0.5f)));
	
	}
	
	// If the Texture is null then no need to draw a bitmap
	if(m_CurrentTexture)
	{
		// Set the current texture
		m_CurrentTexture->PlaySprite();
		// Draw Bitmap
		m_d2dContext->DrawBitmap(m_CurrentTexture->m_Bitmap.Get(),  D2D1::RectF(0,0, m_OriginalWidth, m_OriginalHeight),  1.0f, D2D1_INTERPOLATION_MODE_LINEAR,  D2D1::RectF(m_CurrentTexture->FrameLeft,m_CurrentTexture->FrameTop, m_CurrentTexture->FrameRight,m_CurrentTexture->FrameBotom) );
	}
	else if(m_Text)
	{
		// Drawin the text file 
		m_d2dContext->DrawTextLayout(D2D1::Point2F(0.0f, 0.0f),m_Text->m_textLayout.Get(),m_Text->m_textBrush.Get(),D2D1_DRAW_TEXT_OPTIONS_NO_SNAP);
	}

	position =  Vector2(collisionBox->GetPosition().x, collisionBox->GetPosition().y);
	angle = collisionBox->GetAngle();	
		
}

Vector2 GameObject::GetMainCameraPosition()
{
	return Vector2(-GameObject::p_Main->GetMainCameraPosition()._31/50,GameObject::p_Main->GetMainCameraPosition()._32/50);
}
std::wstring GameObject::GetTextureName()
{
	return m_CurrentTexture->m_TextureName;
	//return L"AAA";
}


void GameObject::RotateToLookAt(float x,float y)
{
	RotateToLookAt(Vector2(x,y));
}
void GameObject::RotateToLookAt(Vector2 _LookAt)
{
	float bodyAngle = collisionBox->GetAngle();
    b2Vec2 toTarget = b2Vec2(_LookAt.x,_LookAt.y) - collisionBox->GetPosition();
    float desiredAngle = atan2f( -toTarget.x, toTarget.y );
	collisionBox->SetTransform( collisionBox->GetPosition(), desiredAngle );
	collisionBox->SetAngularVelocity(0);
}

void GameObject::RotateToLookAtTime(Vector2 _LookAt,float change)
{
	float bodyAngle = collisionBox->GetAngle();
	b2Vec2 toTarget = b2Vec2(_LookAt.x,_LookAt.y) - collisionBox->GetPosition();
    float desiredAngle = atan2f( -toTarget.x, toTarget.y );
	float totalRotation = desiredAngle - bodyAngle;
	change = change * 0.0174532f; //allow 1 degree rotation per time step
	float newAngle = bodyAngle + min( change, max(-change, totalRotation));
	collisionBox->SetTransform( collisionBox->GetPosition(), newAngle );
	collisionBox->SetAngularVelocity(0);
}

void GameObject::TranslateVelocity (float x,float y, Coordinate _RotationCoordinate)
{
	TranslateVelocity(Vector2(x,y),_RotationCoordinate);
}

/// Translate Object relative to Local space or global
void GameObject::TranslateVelocity (Vector2 _Direction, Coordinate _RotationCoordinate)
{
	if(_RotationCoordinate == Local)
	{
		b2Vec2 forward =collisionBox->GetWorldVector(b2Vec2(_Direction.x,_Direction.y));
		collisionBox->SetLinearVelocity(forward);			
	}
	else if(_RotationCoordinate == Global)
	{
		collisionBox->SetLinearVelocity(b2Vec2(_Direction.x, _Direction.y));
	}
}

Vector2 GameObject::GetVelocity()
{
	return Vector2(collisionBox->GetLinearVelocity().x,collisionBox->GetLinearVelocity().y);
}
float GameObject::GetVelocitySize()
{
	return collisionBox->GetLinearVelocity().Length();
}

void GameObject::AddForce (float x,float y, Coordinate _RotationCoordinate)
{
	AddForce(Vector2(x,y),_RotationCoordinate);
}

void GameObject::AddForce(Vector2 _Direction, Coordinate _RotationCoordinate)
{
	if(_RotationCoordinate == Local)
	{
		b2Vec2 forward =collisionBox->GetWorldVector(b2Vec2(_Direction.x,_Direction.y));
		collisionBox->ApplyForceToCenter(forward);
	}
	else if(_RotationCoordinate == Global)
	{
		collisionBox->ApplyForceToCenter(b2Vec2(_Direction.x, _Direction.y));
	}
}

void GameObject::SetDrawScale (Vector2 Scale)
{
	SetDrawScale(Scale.x,Scale.y);
}

void GameObject::SetDrawScale(float x,float y)
{
	x_DrawScale=x;
	y_DrawScale=y;
}



void GameObject::SetCollisionScale(float x,float y)
{
	collisionBox->DestroyFixture(collisionBox->GetFixtureList());
	b2PolygonShape Box;
	Box.SetAsBox((m_OriginalWidth*0.5f*x)/50,( m_OriginalHeight*0.5f*y)/50);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &Box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.8f;
	collisionBox->CreateFixture(&fixtureDef);
	x_CollisionScale=x;
	y_CollisionScale=y;
}

void GameObject::SetCollisionScale (Vector2 Scale)
{
	SetCollisionScale(Scale.x,Scale.y);
}

void GameObject::SetScale(Vector2 &Scale)
{
	SetCollisionScale(Scale.x,Scale.y);
	SetDrawScale(Scale.x,Scale.y);
}


void GameObject::SetScale(float x,float y)
{
	SetDrawScale(x,y);

	//We can't set the collider to exactly 0.0 or the physics engine will assert
	SetCollisionScale((x==0.0f)?0.0001f:x,(y==0.0f)?0.0001f:y);
}
Vector2 GameObject::GetCollisionScale()
{
	return Vector2(x_CollisionScale,y_CollisionScale);
}
Vector2 GameObject::GetDrawScale()
{	
	return Vector2(x_DrawScale,y_DrawScale);
}

void GameObject::RotateAroundALocalPoint(float Angle,Vector2 Point)
{	
	RotateAroundALocalPoint(Angle,Point.x,Point.y);
}

void GameObject::RotateAroundALocalPoint(float Angle, float x, float y)
{	
	b2Vec2 vertexLocalPos=b2Vec2(x,y);//fulcrum vertex in local (body) coords
	b2Vec2 vertexBeforeWorldPos = collisionBox->GetWorldPoint( vertexLocalPos );
	collisionBox->SetTransform( collisionBox->GetPosition(), collisionBox->GetAngle() + Angle );
	b2Vec2 vertexAfterWorldPos =  collisionBox->GetWorldPoint(b2Vec2(x,y));
	collisionBox->SetTransform( collisionBox->GetPosition() + vertexBeforeWorldPos - vertexAfterWorldPos, collisionBox->GetAngle() );
}


Vector2 GameObject::GetGlobalPositionLocalOffset(Vector2 Point)
{	
	return GetGlobalPositionLocalOffset(Point.x,Point.y);
}

Vector2 GameObject::GetGlobalPositionLocalOffset( float x, float y)
{	
	b2Vec2 vertexLocalPos=b2Vec2(x,y);//fulcrum vertex in local (body) coords
	b2Vec2 vertexBeforeWorldPos = collisionBox->GetWorldPoint( vertexLocalPos );
	return Vector2(vertexBeforeWorldPos.x,vertexBeforeWorldPos.y);
}

void GameObject::SetPosition(Vector2 &Point)
{
	SetPosition(Point.x,Point.y);
}

void GameObject::SetPosition(float x,float y)
{
	collisionBox->SetTransform( b2Vec2(x,y), collisionBox->GetAngle());
}


void GameObject::Rotate(float angle)
{
	collisionBox->SetTransform( collisionBox->GetPosition(), collisionBox->GetAngle()+angle);
}

void GameObject::SetRotation(float angle)
{
	collisionBox->SetTransform( collisionBox->GetPosition(), angle);
}

void GameObject::Translate(float x,float y, Coordinate _RotationCoordinate)
{
	Translate(Vector2(x,y),_RotationCoordinate);
}

void GameObject::Translate(Vector2 _Direction, Coordinate _RotationCoordinate)
{
	if(_RotationCoordinate == Local)
	{
		b2Vec2 forward =collisionBox->GetWorldVector(b2Vec2(_Direction.x,_Direction.y));
		collisionBox->SetTransform( collisionBox->GetPosition()+forward, collisionBox->GetAngle());
	}
	else if(_RotationCoordinate == Global)
	{
		collisionBox->SetTransform( collisionBox->GetPosition()+b2Vec2(_Direction.x,_Direction.y), collisionBox->GetAngle());
	}
}
void GameObject::FixRotation(bool flag)
{
	collisionBox->SetFixedRotation(flag);
}

void GameObject::PlaySpriteTexture()
{
	// Run the animation for the texture
	SetTextureFrame(0);
	m_CurrentTexture->ResumeSprite();
}

void GameObject::PauseSpriteTexture()
{
	// Pause the animation for the texture
	m_CurrentTexture->PauseSprite();
}
	
void GameObject::ResumeSpriteTexture()
{
	// Resume the animation for the texture
	m_CurrentTexture->ResumeSprite();
}

// Set the default frame 
void GameObject::SetTextureFrame()
{
	SetTextureFrame(0);
}

// Set the given frame 
void GameObject::SetTextureFrame(int _FrameNumber)
{
	m_CurrentTexture->SetTextureFrame(_FrameNumber);
}

int GameObject::GetCurrentTextureFrame()
{
	return m_CurrentTexture->CurrentFrame;
}

bool GameObject::IsPlayingTexture()
{
	return m_CurrentTexture->IsPlaying();
}

bool GameObject::IsDynamic()
{
	return dynamic;
}

// Setting the font color
void GameObject::SetFontColor(D2D1_COLOR_F _FontColor)
{
	if(m_Text != NULL)
	{
		m_Text->SetFontColor(_FontColor);
	}
}
// Setting the Size of the TextBlock
void GameObject::SetSizeOfTextBlockInPixel(float _Widht, float _Height)
{
	if(m_Text != NULL)
	{
		m_Text->SetSizeOfTextBlockInPixel(_Widht,_Height);
	}
}

// Set Algnment of the text 
void GameObject::SetAlignmentTextBlock(int _Alignment)
{
	if(m_Text != NULL)
	{
		m_Text->SetAlignment(FontAlignment(_Alignment));
	}
}

void GameObject::SetText(std::wstring _Text)
{
	if(m_Text != NULL)
	{
		m_Text->SetText(&_Text);
	}
}

void GameObject::AddSound(wchar_t* filename, int type)
{
	if(p_Audio)
	{
		wchar_t* m_filename = L"Assets/Audio/";
		int a = wcslen(filename) + wcslen(m_filename) + 1;
		wchar_t* buf = new wchar_t[a];
		wcscpy_s(buf, a, m_filename);
		wcscat_s(buf, a, filename);

		Sounds newSound;
		newSound.m_filename = buf;
		newSound.m_type = type;

		p_Audio->CreateSourceVoice(this, buf, SoundEffectType(type));
		m_soundsList.push_back(newSound);
	}
}

void GameObject::PlaySound(wchar_t* soundname)
{
	if(p_Audio)
	{
		wchar_t* m_filename = L"Assets/Audio/";
		int a = wcslen(soundname) + wcslen(m_filename) + 1;
		wchar_t* buf = new wchar_t[a];
		wcscpy_s(buf, a, m_filename);
		wcscat_s(buf, a, soundname);

		Sounds newSound;
		newSound.m_filename = buf;

		p_Audio->PlaySoundEffect(this, buf);
		m_soundsList.push_back(newSound);
	}
}

void GameObject::StopSound(wchar_t* soundname)
{
	if(p_Audio)
	{
		wchar_t* m_filename = L"Assets/Audio/";
		int a = wcslen(soundname) + wcslen(m_filename) + 1;
		wchar_t* buf = new wchar_t[a];
		wcscpy_s(buf, a, m_filename);
		wcscat_s(buf, a, soundname);

		Sounds newSound;
		newSound.m_filename = buf;

		p_Audio->StopSoundEffect(this, buf);
		m_soundsList.push_back(newSound);
	}
}