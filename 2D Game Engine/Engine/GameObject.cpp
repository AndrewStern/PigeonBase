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
}


GameObject::GameObject(std::wstring _textObjectName, std::wstring _FontName, float _FontSize, std::wstring _Text, Main* _Main, Vector2 &_Position)
:	objectActive(true),
	gameObjectId(0),
	RotateTextureWithPhysics(true)
{
	// Setting the gameobject name 
	name = _textObjectName;

	// Ref to main Class 
	p_Main = _Main;

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
	// Set the Collider type to Box
	m_ColliderType = ColliderType::BoxCollider;
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

// Empty GameObject. An Empty Gameobject is a minimum GameObject. It has no Texture, and has a minimum collision scale 
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
	// Set the Collider type to Box
	m_ColliderType = ColliderType::BoxCollider;

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
	fixtureDef.density = 20;
	fixtureDef.friction = 20;
	// Set the Collider type to Box
	m_ColliderType = ColliderType::BoxCollider;

	body->CreateFixture(&fixtureDef);
	collisionBox = body;

	if(!physicsBody)
	{
		collisionBox->SetActive(false);
	}


	// Setting the original Height and width
	m_OriginalWidth = m_CurrentTexture->width;
	m_OriginalHeight = m_CurrentTexture->height;
}

// Gives your Game Object a Tag. Useful for grouping your Objects with a name. For Example you can tag your Environment Objects as "Furniture" or "Explosive", etc
// Which can then be used in various parts of your game. For Example, on collision with other gameobjects, you can test the tag, and if its an "Explosove", you can
// Call a function for simulation explosion etc.
void GameObject::SetTag(std::wstring ChangeTo)
{
	Tag=ChangeTo;
}

// Gives your game object a unique identifier. Please Note, that if more than one game objects has a same name, then the Engine will automatically append an integer
// at the end of the name to distinguish them. Please always try to give unique names to your game objects to make it easy for you to access them.
void GameObject::SetName(std::wstring ChangeTo)
{
	name=ChangeTo;
}

// Sets the Object's gravity scaly. This allows you to set different amount of gravity to different objects.
void GameObject::SetGravity(float gravity)
{
	collisionBox->SetGravityScale(gravity);
}

// Returns the Name of the game object
std::wstring GameObject::GetName()
{
	return name;
}

// Returns the Tag of the game object
std::wstring GameObject::GetTag()
{
	return Tag;
}

// Compares the game object's Tag against the provided string.
// Returns True if they match and False if they do not match
bool GameObject::CompareTag(std::wstring TestTo)
{
	if(TestTo.compare(Tag)==0)
	{
		return true;
	}
	
	return false;
}

// Compares the game object's Name against the provided string.
// Returns True if they match and False if they do not match
bool GameObject::CompareName(std::wstring TestTo)
{
	if(TestTo.compare(name)==0)
	{
		return true;
	}
	return false;
}

// Adds a Texture or Spritesheet to the list of the already existing Textures, if any
void GameObject::AddTexture(std::wstring _TextureName, bool _IsSprite, int _Rows, int _Columns)
{
	
	TextureHolder.push_back(GameObject::p_Main->m_textureManager->AddTexture(GameObject::p_Main, _TextureName, _IsSprite, _Rows, _Columns));	
}

// Sets the current Texture to the one specified if it exists in the game object's texture list
void GameObject::SetTexture(std::wstring _TextureName)
{
	
	for(m_TextureIterator=TextureHolder.begin(); m_TextureIterator!=TextureHolder.end(); m_TextureIterator++)
	{	
		TextureEntry * m_tex = *m_TextureIterator;
		if(m_tex->texture->m_TextureName.compare(_TextureName)==0)
		{	
			// If the texture is null set this texture as the first one 
			if(m_CurrentTexture == NULL)
			{
				m_OriginalWidth  = m_tex->texture->width;
				m_OriginalHeight = m_tex->texture->height;
			}

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
	 collisionBox->GetWorld()->DestroyBody(collisionBox);
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
	if (m_CurrentTexture == NULL)
	{
		return L"";
	}

	return m_CurrentTexture->m_TextureName;
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

// Obselete function. Needs Further implementation
//void GameObject::RotateToLookAtTime(Vector2 _LookAt,float change)
//{
//	float bodyAngle = collisionBox->GetAngle();
//	b2Vec2 toTarget = b2Vec2(_LookAt.x,_LookAt.y) - collisionBox->GetPosition();
//    float desiredAngle = atan2f( -toTarget.x, toTarget.y );
//	float totalRotation = desiredAngle - bodyAngle;
//	change = change * 0.0174532f; //allow 1 degree rotation per time step
//	float newAngle = bodyAngle + min( change, max(-change, totalRotation));
//	collisionBox->SetTransform( collisionBox->GetPosition(), newAngle );
//	collisionBox->SetAngularVelocity(0);
//}

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

// Set the Collision Scale(X,Y) Of the GameObject 
void GameObject::SetCollisionScale(float x,float y)
{
	// Only the Box Colliders Can Scale 
	if(m_ColliderType == ColliderType::BoxCollider)
	{
		float _GetFriction =0;// collisionBox->GetFixtureList()->GetFriction();
		float _GetDensity=1;// collisionBox->GetFixtureList()->GetDensity();

		collisionBox->DestroyFixture(collisionBox->GetFixtureList());
		b2PolygonShape Box;
		Box.SetAsBox((m_OriginalWidth*0.5f*x)/50,( m_OriginalHeight*0.5f*y)/50);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &Box;
		fixtureDef.density = _GetDensity;
		fixtureDef.friction = _GetFriction;
		collisionBox->CreateFixture(&fixtureDef);
		x_CollisionScale=x;
		y_CollisionScale=y;
	}
	else
	{
		assert(false && "You can only scale Box Colliders...");  
	}


}

// Set the Collision Scale(Vector2) Of the GameObject 
void GameObject::SetCollisionScale (Vector2 Scale)
{
	SetCollisionScale(Scale.x,Scale.y);
}

// Set the GameObject Draw And Collision Scale (Vector2)
void GameObject::SetScale(Vector2 &Scale)
{
	SetCollisionScale(Scale.x,Scale.y);
	SetDrawScale(Scale.x,Scale.y);
}

// Set the GameObject Draw And Collision Scale (X,Y)
void GameObject::SetScale(float x,float y)
{
	SetDrawScale(x,y);

	//We can't set the collider to exactly 0.0 or the physics engine will assert
	SetCollisionScale((x==0.0f)?0.0001f:x,(y==0.0f)?0.0001f:y);
}

// Return the Collision Scale of the Gameobject 
Vector2 GameObject::GetCollisionScale()
{
	return Vector2(x_CollisionScale,y_CollisionScale);
}

// Return the Draw Scale of the Gameobject 
Vector2 GameObject::GetDrawScale()
{	
	return Vector2(x_DrawScale,y_DrawScale);
}

// Rotate GameObject in a given point(Vector2) and angle 
void GameObject::RotateAroundALocalPoint(float Angle,Vector2 Point)
{	
	RotateAroundALocalPoint(Angle,Point.x,Point.y);
}

// Rotate GameObject in a given point(X,Y) and angle 
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

Vector2 GameObject::GetGlobalPositionLocalOffset(float x, float y)
{	
	b2Vec2 vertexLocalPos=b2Vec2(x,y);//fulcrum vertex in local (body) coords
	b2Vec2 vertexBeforeWorldPos = collisionBox->GetWorldPoint( vertexLocalPos );
	return Vector2(vertexBeforeWorldPos.x,vertexBeforeWorldPos.y);
}

// Set the position(Vector2) of the GameObject 
void GameObject::SetPosition(Vector2 &Point)
{
	SetPosition(Point.x,Point.y);
}

// Set the position(X,V) of the GameObject 
void GameObject::SetPosition(float x,float y)
{
	collisionBox->SetTransform(b2Vec2(x,y), collisionBox->GetAngle());
}

// Rotate Around GameObject Center point 
void GameObject::Rotate(float angle)
{
	collisionBox->SetTransform( collisionBox->GetPosition(), collisionBox->GetAngle()+angle);
}

// Set the rotation of the GameObject 
void GameObject::SetRotation(float angle)
{
	collisionBox->SetTransform( collisionBox->GetPosition(), angle / 57.2957795f); // Convert Angle From Radians to Degrees 
}

// Translate the game object to a given point(X,Y) in local or global coordinate 
void GameObject::Translate(float x,float y, Coordinate _RotationCoordinate)
{
	Translate(Vector2(x,y),_RotationCoordinate);
}

// Translate the game object to a given point(Vector2) in local or global coordinate 
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

// Set the GameObject to rotate with the physics or not  
void GameObject::FixRotation(bool flag)
{
	collisionBox->SetFixedRotation(flag);
}

// Run the animation for the texture
// Animation will start from the 0th frame 
void GameObject::PlaySpriteTexture()
{	
	SetTextureFrame(0);
	m_CurrentTexture->ResumeSprite();
}

// Pause the animation for the texture
void GameObject::PauseSpriteTexture()
{	
	m_CurrentTexture->PauseSprite();
}
	
// Resume the animation for the texture
void GameObject::ResumeSpriteTexture()
{	
	m_CurrentTexture->ResumeSprite();
}

// Set the default frame 
void GameObject::SetTextureFrame()
{
	SetTextureFrame(0);
}

// Setting the animation speed of sprite texture 
void GameObject::SetAnimationSpeed(float _Speed)
{
	m_CurrentTexture->SetFrameSpeed(_Speed);
}

// Set the given frame 
void GameObject::SetTextureFrame(int _FrameNumber)
{
	m_CurrentTexture->SetTextureFrame(_FrameNumber);
}

// Return the current Texture Frame number
int GameObject::GetCurrentTextureFrame()
{
	return m_CurrentTexture->CurrentFrame;
}

// Is the Texture Playing 
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

// Set Alignment of the text  
void GameObject::SetAlignmentTextBlock(int _Alignment)
{
	if(m_Text != NULL)
	{
		m_Text->SetAlignment(FontAlignment(_Alignment));
	}
}

// Set the text for Text GameObject
void GameObject::SetText(std::wstring _Text)
{
	if(m_Text != NULL)
	{
		m_Text->SetText(&_Text);
	}
}

void GameObject::AddSound(wchar_t* filename, int type)
{
	m_soundsList.push_back(GameObject::p_Main->m_audioManager->AddSound(filename, type));	
}

void GameObject::PlaySound(wchar_t* soundname)
{
	GameObject::p_Main->m_audioManager->PlaySound(soundname);
}

void GameObject::StopSound(wchar_t* soundname)
{
	GameObject::p_Main->m_audioManager->StopSound(soundname);
}


// RayCasting from game object position
// 1 radian = 57.2957795 degrees
bool GameObject::RayCast(float _Angle, float _Lenght, wstring Tag)
{
	b2RayCastInput input;
	input.p1 = b2Vec2(this->position.x, this->position.y);
	input.p2 = b2Vec2(sin(_Angle*0.01745f)+this->position.x, cos(_Angle*0.01745f)+this->position.y);
	input.maxFraction = _Lenght;

	GameObject* object1; 

	for(b2Body* b=world->GetBodyList(); b; b=b->GetNext())
	{
		object1 = (GameObject*)b->GetUserData();
		if(object1->CompareTag(Tag))
		{
			if(b!=collisionBox)
			{
				for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
				{
					b2Transform transform;
					transform.SetIdentity();
					b2RayCastOutput output;
					if (f->RayCast(&output, input, 0 ))
					{
						return true;
					}  
				}
			}
		}
	}
	return false;

}

// RayCasting from given position
bool  GameObject::RayCast(Vector2 _StartPoint,float _Angle, float _Lenght, wstring Tag)
{
	b2RayCastInput input;
	input.p1 = b2Vec2(_StartPoint.x, _StartPoint.y);
	input.p2 = b2Vec2(sin(_Angle*0.01745f)+_StartPoint.x, cos(_Angle*0.01745f)+_StartPoint.y);
	input.maxFraction = _Lenght;

	GameObject* object1; 

	for(b2Body* b=world->GetBodyList(); b; b=b->GetNext())
	{
		object1 = (GameObject*)b->GetUserData();
		if(object1->CompareTag(Tag))
		{
			if(b!=collisionBox)
			{
				for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
				{
					b2Transform transform;
					transform.SetIdentity();
					b2RayCastOutput output;
					if (f->RayCast(&output, input, 0 ))
					{
						return true;
					}  
				}
			}
		}
	}
	return false;
}

// Retrun the Width(X) and Height (Y) of the gameobject
Vector2 GameObject::GetWidthAndHeight()
{
	return Vector2((m_OriginalWidth * 0.5f)/50,(m_OriginalHeight * 0.5f)/50);
}

// Trigger the collider 
void GameObject::IsTrigger(bool _Trigger)
{
	collisionBox->GetFixtureList()->SetSensor(_Trigger);
}

// Return the trigger information
bool GameObject::IsTriggered()
{
	return collisionBox->GetFixtureList()->IsSensor();
}

// Return the current gameobject angle
float GameObject::GetRotationAngle()
{
	// 1 radian = 57.2957795 degree
	return (angle * 57.2957795f);
}

// Set the Friction Of the Game Object
void GameObject::SetFriction(float _Friction)
{
	collisionBox->GetFixtureList()->SetFriction(_Friction);
	collisionBox->GetFixtureList()->Refilter();
}

// Set the Density Of the Game Object
void GameObject::SetDensity(float _Density)
{
	collisionBox->GetFixtureList()->SetDensity(_Density);
	collisionBox->ResetMassData();
}

// Set the damping of the object
// Damping is used to reduce the world velocity of gameobject
void GameObject::SetDamping(float Damping)
{
	collisionBox->SetLinearDamping(Damping);
}

// Set the Sphere collision's to GameObject
void GameObject::SetCollisionToSphere(float _Radius)
{
	 float density=collisionBox->GetFixtureList()->GetDensity();
	 float friction=collisionBox->GetFixtureList()->GetFriction();
	 collisionBox->DestroyFixture(collisionBox->GetFixtureList());
	 b2CircleShape circle ;
	 circle.m_radius = _Radius;

	 b2FixtureDef fixtureDef;
	 fixtureDef.shape = &circle;
	 fixtureDef.density = density;
	 fixtureDef.friction = friction;
	 // Set the Collider type to Sphere
	 m_ColliderType = ColliderType::SphereCollider;

	 collisionBox->CreateFixture(&fixtureDef);
	 x_CollisionScale=_Radius;
	 y_CollisionScale=_Radius;
}

// Set the Custom Collision's to GameObject
void GameObject::SetCollisionToCustom(const b2Vec2* vertices,int _NumberVertices)
{
	 float density=collisionBox->GetFixtureList()->GetDensity();
	 float friction=collisionBox->GetFixtureList()->GetFriction();
	 collisionBox->DestroyFixture(collisionBox->GetFixtureList());
	 b2PolygonShape polygonShape;
	 polygonShape.Set(vertices, _NumberVertices);

	 b2FixtureDef fixtureDef;
	 fixtureDef.shape = &polygonShape;
	 fixtureDef.density = density;
	 fixtureDef.friction = friction;
	 // Set the Collider type to Custom
	 m_ColliderType = ColliderType::CustomCollider;

	 collisionBox->CreateFixture(&fixtureDef);
	 x_CollisionScale=1;
	 y_CollisionScale=1;
}

