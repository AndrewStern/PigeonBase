#pragma once
#include "engine\gameobject.h"

class Ground :public GameObject
{
	void Collided(GameObject*);
	void Start();
	void Update(unsigned long frameNumber);
	std::wstring GetType();
	public:
		Ground() : GameObject(){}
		Ground(std::wstring objectName, Main* _Main, bool dynamic, bool physicsBody, Vector2 &_Position, LPCWSTR _TextureName, bool _IsSprite, int _Rows, int _Columns) : GameObject(objectName, _Main, dynamic, physicsBody, _Position, _TextureName, _IsSprite, _Rows, _Columns) {}
};