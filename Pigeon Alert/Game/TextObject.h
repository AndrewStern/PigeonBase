#pragma once
#include "engine\gameobject.h"

class TextObject :public GameObject
{
	void Collided(GameObject*);
	void Start();
	void Update(unsigned long frameNumber);
	std::wstring GetType();
	public:
		TextObject() : GameObject(){}
		TextObject(std::wstring _objectName, std::wstring _FontName, float _FontSize, std::wstring _Text, Main* _Main, Vector2 &_Position) : GameObject(_objectName, _FontName, _FontSize, _Text,_Main, _Position) {}
};