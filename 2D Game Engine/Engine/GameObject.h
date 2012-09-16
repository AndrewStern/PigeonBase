#pragma once
#include "pch.h"
#include "Main.h"
#include "Texture.h"
#include "Layer.h"
#include "Text.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "AudioEngine.h"

class Main;
class Texture;
class Layer;
class Text;
struct TextureEntry;

/**
 * An enum that defines the types of coordinates
 */
enum Coordinate
{
	Local, /** Local coordinates specific to the object */
	Global /** Global or world coordinates */
};

/**
 * An enum that defines the types or shapes of the Collision body or the collider
 */
enum ColliderType
{
	BoxCollider,		/** Box/Rectangular shaped collider */
	SphereCollider,		/** Sphere/Circle shaped collider */
	CustomCollider		/** Custom/User-Defined shaped collider */
};


/**
 * The GameObject class is by far the Most Important class in the entire engine. The engine is extremely Game-Object Oriented Engine. This means, 
 * that the engine expects almost everything in your game to either be a GameObject or a sub-class of GameObject. This does not mean that you cannot
 * directly use Direct2D etc, to draw or implement your elements, but the engine will not know how to handle them or be able to apply physics to them
 * unless you explicitly specify/code/program how they should be handled.
 *
 * NOTE: It is strongly recommended that you always use GameObjects or create sub-classes of GameObject to create your in game elements/objects.
 */
class GameObject
{
public:
	/**
	 * Default Constructor
	 */
	GameObject();

	/**
	 * String GameObject: Creates a gameobject that has no physcis and is capable of rendering a string to the screen.
	 * 
	 * @param		_textObjectName		Unique Name or Identifier of this game object
	 * @param		_FonttName			The Name of the Font to be used when drawing the string to the screen
	 * @param		_FontSize			The Size of the Font
	 * @param		_Text				The String to be drawn to the screen
	 * @param		_Main				Reference of the Main/UserMain class. Required for drawing the string to the screen
	 * @param		&_position			The on-screen position of the game object or text
	 */
	GameObject(std::wstring _textObjectName, std::wstring _FontName, float _FontSize, std::wstring _Text, Main* _Main, Vector2 &_Position);

	/**
	 * Empty GameObject: Creates an empty gameobject that has no default texture, and has a collision box scale of (1,1) 
	 * 
	 * @param		objectName			Unique Name or Identifier of this game object
	 * @param		_Main				Reference of the Main/UserMain class. Required for drawing the string to the screen
	 * @param		dynamic				is this object a dynamic physics body
	 * @param		physics				is this object a physics body
	 * @param		&_position			The on-screen position of the game object
	 */
	GameObject(std::wstring objectName, Main* _Main, bool dynamic, bool physicsBody, Vector2 &_Position);

	/**
	 * Textured GameObject: Creates a gameobject that has a default texture/spritesheet to start with 
	 * 
	 * @param		_textObjectName		Unique Name or Identifier of this game object
	 * @param		_FonttName			The Name of the Font to be used when drawing the string to the screen
	 * @param		_FontSize			The Size of the Font
	 * @param		_Text				The String to be drawn to the screen
	 * @param		_Main				Reference of the Main/UserMain class. Required for drawing the string to the screen
	 * @param		&_position			The on-screen position of the game object or text
	 */
	GameObject(std::wstring objectName, Main* _Main, bool dynamic, bool physicsBody, Vector2 &_Position, std::wstring _TextureName, bool _IsSprite, int _Rows, int _Columns);

	/**
	 * Sets the Gravity offset for this game object
	 * @param		gravity			The offset value of this objects gravity
	 */
	void SetGravity(float gravity);	

	/**
	 * This function is called by the engine each frome on every gameobject alive in the game
	 *
	 * @param		m_d2dContext		A reference to the Direct2D device context, used by the engine to render the game object
	 */
	void Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_d2dContext);

	/**
	 * This is a virtual function that should be implemented by the GameObject sub-classes.
	 * The Update function of each active gameobject is called by the engine before the Render function
	 *
	 * @param		frameNumber		The value of the current frame
	 */
	virtual void Update(unsigned long frameNumber) {}

	/**
	 * This is a virtual function that should be implemented by the GameObject sub-classes.
	 * The Start function of each gameobject is called by the engine immediately after the gameobject is created.
	 * Use this function to initialize your gameobjects and their members. Do NOT use the constructors of your sub-classes for initializing
	 * your game objects.
	 */
	virtual void Start() {}

	/**
	 * This is a virtual function that should be implemented by the GameObject sub-classes.
	 * The Engine calls the Collided function when it detects collision between this gameobject and another gameobject in the game
	 *
	 * @param		_Object			The game object that this object collided with
	 */
	virtual void Collided(GameObject * _Object) {}

	/**
	 * This is a virtual function that should be implemented by the GameObject sub-classes
	 *
	 * @return		Name of the class
	 */
	virtual std::wstring GetType() { return L"GameObject"; }

	/**
	 * This is called by the engine when an object receives a Touch Input of click input
	 *
	 * @param		_Param		The point in the world where the press event took place
	 */
	virtual void OnPointerPressed(Vector2 _Point){}

		/**
	 * Is called by the engine when this object receives Drag input via 'Touch and Drag' or 'Click and Drag'.
	 *
	 * @param		_Point		the current touch point in the world, updated each frame until touch/click is released
	 */
	virtual void OnPointerMoved(Vector2 _Point){}

	/**
	 * Adds a force to the game object in x-axis and y-axis to the specified coordinate system.
	 *
	 * @param		x						force in x-axis
	 * @param		y						force in y-axis
	 * @param		_RotationCoordinate		Coordinates to add force to. Global or Local
	 */
	void AddForce (float x,float y, Coordinate _RotationCoordinate);

	/**
	 * Adds a force to the game object in x-axis and y-axis to the specified coordinate system.
	 *
	 * @param		_Direction				A Vector2 object that holds x,y values. Adds force in the x and y axis based on the x and y values of the Vector2 object	
	 * @param		_RotationCoordinate		Coordinates to add force to. Global or Local
	 */
	void AddForce(Vector2 _Direction, Coordinate _RotationCoordinate);

	/**
	 * Translates or moves the gameobject in the given direction of the specified coordinate system.
	 *
	 * @param		Direction				The Direction or offset to move the object to. Ex: (2,3) -> moves the object 2 units on the x- axis and 3 units in the y-axis
	 * @param		RotationCoordinate		Coordinates to translate the object in. Global or Local
	 */
	void Translate(Vector2 Direction, Coordinate RotationCoordinate);

	/**
	 * Translates or moves the gameobject in the given direction of the specified coordinate system.
	 * 
	 * @param		x						units in x-axis to translate/move the object 
	 * @param		y						units in y-axis to translate/move the object
	 * @param		RotationCoordinate		Coordinates to translate the object in. Global or Local
	 */
	void Translate(float x,float y, Coordinate _RotationCoordinate);
	
	/**
	 * Translates or modifies the linear velocity of the object in x and y axis in the specified coordinate system
	 * 
	 * @param		Direction				The offset to add the object's velocity to. Ex: (2,3) -> adds 2 units on the x- axis and 3 units in the y-axis to the objects velocity
	 * @param		RotationCoordinate		Coordinates to translate the velocity in. Global or Local
	 */
	void TranslateVelocity(Vector2 Direction, Coordinate RotationCoordinate);

	/**
	 * Translates or modifies the linear velocity of the object in x and y axis in the specified coordinate system
	 *
	 * @param		x						x-axis value to add to the velocity
	 * @param		y						y-axis value to add to the velocity
	 * @param		RotationCoordinate		Coordinates to translate the velocity in. Global or Local
	 */
	void TranslateVelocity (float x,float y, Coordinate _RotationCoordinate);

	/**
	 * Rotates the game object to look at a point(Vector2(x,y)) in the game world
	 *
	 * @param		_LookAt			Point in the world to look at
	 */
	void RotateToLookAt(Vector2 _LookAt);

	/**
	 * Rotates the game object to look at a point(float x, float y) in the game world
	 *
	 * @param		x		x-axis value to look at
	 * @param		y		y-axis value to look at
	 */
	void RotateToLookAt(float x,float y);

	/**
	 * Rotates the object around the given pivot point rathar than the the default point (Center of the object).
	 *
	 * @param		Angle		The angle in degrees to rotate the object by.
	 * @param		Point		The point in the world to use as the pivot point to rotate the object around		
	 */
	void RotateAroundALocalPoint(float Angle,Vector2 Point);

	/**
	 * Rotates the object around the given pivot point rathar than the the default point (Center of the object).
	 *
	 * @param		Angle		The angle in degrees to rotate the object by.
	 * @param		x			The x-axis point in the world to use as the pivot point to rotate the object around		
	 * @param		y			The y-axis point in the world to use as the pivot point to rotate the object around		
	 */
	void RotateAroundALocalPoint(float Angle, float x, float y);

	/*
	 * Obselete function. Needs Further implementation
	 *
	 *  void RotateToLookAtTime(Vector2 _LookAt,float change);
	 */

	/**
	 * Sets the size of the object's texture without changing the collision box size
	 * 
	 * @param		x		size in x-axis
	 * @param		y		size in y axis
	 */
	void SetDrawScale(float x,float y);

	/**
	 * Sets the size of the object's texture without changing the collision box size
	 *
	 * @param		Scale		size in x and y axis 
	 */
	void SetDrawScale (Vector2 Scale);

	/**
	 * Sets the size of the collision box without changing the size of the texture
	 * Note: This function only works on the default collision shape i.e., a 'Box' shape collider. 
	 * Using this function on a custom collider shape or a sphere collider shape or any other collider shape
	 * will give you an assertion failed error
	 *
	 * @param		Scale		The size to set the collision box to
	 */
	void SetCollisionScale (Vector2 Scale);

	/**
	 * Sets the size of the collision box without changing the size of the texture
	 * Note: This function only works on the default collision shape i.e., a 'Box' shape collider. 
	 * Using this function on a custom collider shape or a sphere collider shape or any other collider shape
	 * will give you an assertion failed error
	 *
	 * @param		x		size in x-axis
	 * @param		y		size in y axis
	 */
	void SetCollisionScale(float x,float y);

	/**
	 * Sets the size of both the Texture and the collision box to the given size
	 * 
	 * @param		&Scale			The size to set the collision box and the texture to
	 */
	void SetScale(Vector2 &Scale);

	/**
	 * Sets the size of both the Texture and the collision box to the given size
	 *
	 * @param		x		size in x-axis
	 * @param		y		size in y axis
	 */
	void SetScale(float x,float y);

	/**
	 * Return the current position of the main game camera
	 *
	 * @return		Current Position of the game's main camera
	 */
	Vector2 GetMainCameraPosition();

	/**
	 * Returns Box2D world position as game world position
	 *	
	 * @param		Point		The Box2D world point to convert to game world point
	 * @return					The gamw world point of the Box2D point passed
	 */
	Vector2 GetGlobalPositionLocalOffset(Vector2 Point);

	/**
	 * Returns Box2D world position as game world position
	 *	
	 * @param		x			The Box2D world point's x-axis value 
	 * @param		y			The Box2D world point's y-axis value 
	 * @return					The gamw world point of the Box2D point passed
	 */
	Vector2 GetGlobalPositionLocalOffset(float x, float y);

	/**
	 * Sets the Position of the game object to the specified point in the game world
	 *
	 * @param		&Point		The point to set the game object's position to.
	 */
	void SetPosition(Vector2 &Point);

	/**
	 * Sets the Position of the game object to the specified point in the game world
	 *
	 * @param		x		The x-axis position to set the game object's position to.
	 * @param		y		The y-axis position to set the game object's position to.
	 */
	void SetPosition(float x,float y);

	/**
	 * Returns a Vector2 object containing the current velocity of the game object
	 *
	 * @return		The current velocity of the object
	 */
	Vector2 GetVelocity();

	/**
	 * Returns the normalized velocity as a float
	 *
	 * @return		The normalized value of the velocity vector
	 */
	float GetVelocitySize();

	/**
	 * Sets the objects linear velocity to (x,y)
	 *
	 * @param		x		x-axis velocity
	 * @param		y		y-axis velocity
	 */
	void SetVelocity(float x,float y);

	/**
	 * Returns the  collision scale vector
	 *
	 * @return		Collision scale vector
	 */
	Vector2 GetCollisionScale();

	/**
	 * Returns current texture draw scale vector.
	 * Note; This returns the scale of the current texture that was set using the SetDrawScale function. This does NOT return the actual
	 * size of the texture. To get the actual size of the texture, use the GetWidthAndHeight
	 *
	 * @return		The normalized value of the texture draw scale scale vector
	 */
	Vector2 GetDrawScale();	

	/**
	 * This function returns the current angle of the game object in degrees
	 *
	 * @return Rotation angle of the game object
	 */
	float GetRotationAngle();

	/**
	 * Rotates the object by an angle offset specified in degrees. 
	 * Ex: If the angle already has a rotation of 25 degrees, and we call Rotate(90) on it, it add 90 degrees to the current rotation.
	 * So the object will know have a Rotation of 115 degrees
	 *
	 * @param		angle		angle offset to add to the current rotation.
	 */
	void Rotate(float angle);

	/**
	 * Sets the rotation of the object to the given angle. This function sets the rotation of the object to the specified rotaion
	 * irrespective of its previous rotation angle
	 *
	 * @param		angle		angle in degrees to set the object's rotaion to.
	 */
	void SetRotation(float angle);

	/**
	 * Fixes and unfixes the rotaion of the game object. 
	 * Fixing the rotation makes sure that physics does not effect the rotaion of the object.
	 * For example, colliding with some object, will not effect the rotation of this object if the rotation of this object is fixed.
	 * Note: You can still use the SetRotation and Rotate functions to manually modify the rotations of this object.
	 * 
	 * @param		flag		true = fix rotation, false = un-fix rotation
	 */
	void FixRotation(bool flag);

	/**
	 * Sets and unsets the physics body simulation of this object.
	 *
	 * @param		flag		true = simulate physics, false = do not simulate physics
	 */
	void SetPhysicsBody(bool flag);

	/**
	 * Sets the physics body to a dynamyc or static body
	 *
	 * @param		flag		true = dynamic body, false = static body
	 */
	void SetDynamicBody(bool flag);

	/**
	 * Gives your object a tag. This allows you to group different objects of the same type to different groups.
	 *
	 * @param		ChangeTo		The string value to set as the object's Tag
	 */
	void SetTag(std::wstring ChangeTo);

	/**
	 * Gives your object a unique identifier.
	 * Note: No two objects in your game can have the same name. If you give 2 or more objects the same name,
	 * the engine will append an integer to the end of the name. This will make it hard for you to access your objects
	 * using their names. Always try to give your objects unique names.
	 *
	 * @param		ChangeTo		The new string value to give to your object's Name
	 */
	void SetName(std::wstring ChangeTo);

	/**
	 * Returns the unique name of this object.
	 * @return		Name of the object
	 */
	std::wstring GetName();

	/**
	 * Returns the tag name of this object.
	 * @return		Tag of the object
	 */
	std::wstring GetTag();

	/**
	 * Fires a ray from the object's position to the length given, in the angle specified. 
	 * It then checks for all the objects within the ray with the tag given.
	 * If it finds an object with a matching tag withing the ray, it returns true or else returns false
	 *
	 * @param		_Angle		Angle in which to cast the ray
	 * @param		_Length		The lenght of the ray to cast
	 * @param		Tag			The Tag value to check against the objects
	 * @return		true = if objects found in ray, false = no objects in ray
	 */
	bool RayCast(float _Angle, float _Lenght, wstring Tag);

	/**
	 * Fires a ray from the given position to the given length, in the angle specified. 
	 * It then checks for all the objects within the ray with the tag given.
	 * If it finds an object with a matching tag withing the ray, it returns true or else returns false
	 *
	 * @param		_StartPosition		The starting position of the ray
	 * @param		_Angle				Angle in which to cast the ray
	 * @param		_Length				The lenght of the ray to cast
	 * @param		Tag					The Tag value to check against the objects
	 * @return							true = if objects found in ray, false = no objects in ray
	 */
	bool RayCast(Vector2 _StartPoint,float _Angle, float _Lenght, wstring Tag);

	/**
	 * Returns the actual width and the actual height of the object's current texture, as a Vector2 object
	 *
	 * @return			Vector2(Width, Height) of current texture.
	 */
	Vector2 GetWidthAndHeight();

	/**
	 * Sets the object's collider to a sphere with the given radious rather than the default box shaped collider.
	 *
	 * @param		_Radious		The radious of the sphere collider
	 */
	void SetCollisionToSphere(float _Radius);

	/**
	 * Sets the object's collider to a custom shape with the given vertices rather than the default box shaped collider.
	 *
	 * @param		vertices			A dynamic array of vertices
	 * @param		_NumberVertices		Number of vertices
	 */
	void SetCollisionToCustom(const b2Vec2* vertices,int _NumberVertices);

	/**
	 * Sets the friction of the game object.
	 *
	 * @param		_Friction		The amount of friction to apply to this object
	 */
	void SetFriction(float _Friction);

	/**
	 * Sets a density to the game object
	 *
	 * @param		_Density		FThe amount of density to apply to this object
	 */
	void SetDensity(float _Density);

	/**
	 * Makes this physics body into a trigger. A trigger only logically collides with other objects, but not physically.
	 * This means that it allows the engine to call the Collided function on the gameobjects involved in a collision, but the
	 * objects themselves are not physycally effected by a collision.
	 *
	 * @param		_Trigger		true = makes the object a trigger, false = makes the object a normal physics body.
	 */
	void IsTrigger(bool _Trigger);

	/**
	 * Returns whether or not this object is a trigger
	 *
	 * @return		true if object is a trigger, false if object is not a trigger
	 */
	bool IsTriggered();

	//<--- Texture Setting
	/**
	 * Add a texture to the object's list of textures
	 *
	 * @param		m_TextureName		Filename of the new texture to add
	 * @param		_IsSprite			is this texture an animated sprite
	 * @param		_Rows				How many Rows does this texture have
	 * @param		_Columns			How Many Columns does this texture have
	 */
	void AddTexture(std::wstring m_TextureName, bool _IsSprite, int _Rows, int _Columns);		

	/**
	 * Sets the current texture of the object to the specified texture.
	 *
	 * @param		m_TextureName		Filename of the texture to set
	 */
	void SetTexture(std::wstring m_TextureName);

	/**
	 * Plays the animation of the current texture if it has any animation, that is if it is a sprite sheet
	 */
	void PlaySpriteTexture();

	/**
	 * Pauses animating the current sprite/texture
	 */
	void PauseSpriteTexture();

	/**
	 * Resumes playing the animation of the current sprite/texture
	 */
	void ResumeSpriteTexture();

	/**
	 * Sets the texture/spritesheet's frame to the default frame. i.e., the 0 frame
	 */
	void SetTextureFrame();

	/**
	 * Sets the texture/spritesheet's frame to the specified frame.
	 *
	 * @param		_FrameNumber		The frame to set the spritesheet/texture to.	
	 */
	void SetTextureFrame(int _FrameNumber);

	/**
	 * Sets the animation speed or frame rate of your sprite sheet.
	 *
	 * @param		_Speed		Speed of the animation
	 */
	void SetAnimationSpeed(float _Speed);

	/**
	 * Given the index of the current frame of the spritesheet.
	 *
	 * @return		The current frame
	 */
	int GetCurrentTextureFrame();

	/**
	 * Gets the name of the current texture/spritesheet
	 *
	 * @return		Name of the Current texture
	 */
	std::wstring GetTextureName();

	/**
	 * Returns whether or not the current texture's ani ation is playing or not
	 *
	 * @return		true = playing animation, false = not playing animation
	 */
	bool IsPlayingTexture();

	/**
	 * If true, then the texture is rotated along with the collider. 
	 */
	bool RotateTextureWithPhysics;
	//--->

	//<----- Font Setting 
	/**
	 * Sets the color of the font for the Text Game Object
	 * 
	 * @param		_FontColor			Color of the font
	 */
	void SetFontColor(D2D1_COLOR_F _FontColor);

	
	/**
	 * Sets the Size of the TextBlock for the Text Game Object
	 *
	 * @param		_Width		widht in pixels
	 * @param		_Height		height in pixels
	 */
	void SetSizeOfTextBlockInPixel(float _Widht, float _Height);
 
	/**
	 * Set Algnment of the text
	 *
	 * @param		_Alignment		text alignment value
	 */
	void SetAlignmentTextBlock(int _Alignment);

	/**
	 * Sets the text to render to the screen
	 *
	 * @param		_Text		The value of the string to render
	 */
	void SetText(std::wstring _Text);
	//------>

	
	/**
	 * Compares the object's tag with the parameter.
	 *
	 * @param		TestTo		the string value to compare the tag with
	 * @return		true = match, false = no match
	 */
	bool CompareTag(std::wstring TestTo);

	/**
	 * Compares the object's Name with the parameter.
	 *
	 * @param		TestTo		the string value to compare the Name with
	 * @return		true = match, false = no match
	 */
	bool CompareName(std::wstring TestTo);
	
	/**
	 * Is this object active. If not do not simulate physics
	 */
	bool objectActive;

	/**
	 * Returns whether this object is a dynamic body or not.
	 *
	 * @return		true = dynamic body, false = static body
	 */
	bool IsDynamic();

	/**
	 * Returns whether or not this object is a physics body or not
	 *
	 * @return		true = is a physics body, false = is not a physics body.
	 */
	bool IsPhysicsBody();

	/**
	 * Adds a sound to the list of sounds of this object if the sound does not already exist
	 *
	 * @param		filename		Name of the audio file to add to the gameobject's sounds list
	 * @param		type			The type of sound this is. This value should be 0 or 1, where 0 = Single sound, meaning that this
	 * sound is played only once. and 1 = Continous sound, which means that this sound is looped infinitly until you manually call the stop sound on it
	 */
	void AddSound(wchar_t* filename, int type);

	/**
	 * This function searches through the list of sounds for the mentioned sound, and plays the sound if it finds the name in the list.
	 *
	 * @param		soundname		name of the sound to play
	 */
	void PlaySound(wchar_t* soundname);

	/**
	 * Stops the specified sound if it is currently playing
	 *
	 * @param		soundname		the name of the sound to stop playing
	 */
	void StopSound(wchar_t* soundname);

	

	/**
	 * The Object's Id
	 */
	int gameObjectId;

	/**
	 * Returns the layer for the specified index
	 *
	 * @param		layer		Index of the layer to get.
	 * @return		The layer at index mentioned
	 */
	Layer *  GetLayer(int layer);

	/**
	 * Gets the layer in which this object is in
	 *
	 * @return		The layer of this object
	 */
	Layer *  GetMyLayer();

	/**
	 * Destroys the object
	 */
	void Destroy();

	/**
	 * Called after the Destroy gets executed. Releases any memory resources this object uses.
	 */
	void OnDestroy();

	/**
	 * Current Transform
	 */
	D2D1::Matrix3x2F	m_Transform;	

	/**
	 * The collision body or the collider of this object
	 */
	b2Body*				collisionBox;	

	/**
	 * The index value of the object's layer
	 */
	int Layer;


	/**
	 * The current position of the object
	 */
	Vector2	position;

	/**
	 * Reference of the Main class object
	 */
	static Main*		p_Main;

	/**
	 * Applies a damping value to the  velocity of the object
	 */
	void SetDamping(float Damping);

protected:
	/**
	 * Name of the object
	 */
	std::wstring			name;

	/**
	 * Tag of the object
	 */
	std::wstring			Tag;

	/**
	 * The screen resolution
	 */
	D2D1_SIZE_F		screenSize;

	/**
	 * Screen width
	 */
    int				widthScreen;

	/**
	 * Screen height
	 */
    int				heightScreen;
	
	/**
	 * matrix rotation of the game object
	 */
	D2D1::Matrix3x2F rot2;//=D2D1::Matrix3x2F::Identity();

	/**
	 * Is this object a dynamic body
	 */
	bool	dynamic;

	/**
	 * Is this object a physics body
	 */
	bool	physicsBody;
	
	/**
	 * Screen Offset in x-axis
	 */
	float	offsetX;

	/**
	 * Screen Offset in y-axis
	 */
	float	offsetY;

	/**
	 * Screen x-axis position of the texture, in pixels
	 */
 	float	drawPositionX; 

	/**
	 * Screen x-axis position of the texture, in pixels
	 */
	float	drawPositionY;

	/**
	 * x-axis scale of the texture
	 */
	float   x_DrawScale;

	/**
	 * y-axis scale of the texture
	 */
	float	y_DrawScale;

	/**
	 * x-axis collision scale
	 */
	float	x_CollisionScale;

	/**
	 * y-axis collision scale
	 */
	float	y_CollisionScale;

	/**
	 * The original width of the first texture
	 */
	float	m_OriginalWidth;

	/**
	 * The original height of the first texture
	 */
	float	m_OriginalHeight;

	/**
	 * the current angle of rotation of the object
	 */
	float32	 angle;

	/**
	 * The point in the world where touch happened on this object. Note that an object can cover an area of more than one point.
	 */
	Vector2  touchPoint;

	/**
	 * A list that stores all the textures/spritesheets of this object
	 */
	list<TextureEntry *> TextureHolder;

	/**
	 * An iterator for traversing the TextureHolder list
	 */
	list<TextureEntry *>::iterator m_TextureIterator;

	// Current texture using 
	/**
	 * The current texture of the object
	 */
	Texture*				m_CurrentTexture;

	/**
	 * The current string or text to be rendered on the screen, of the Text game object
	 */
	Text*					m_Text;

	/**
	 * A list containing all the sounds of this object
	 */
	list<AudioEntry *> m_soundsList;

	/**
	 * An iterator for traversing the m_soundList list
	 */
	list<AudioEntry *>::iterator m_soundsListIterator;

	// GameObject Collider Type 
	/**
	 * The current shape of the collider
	 */
	ColliderType m_ColliderType;
};
