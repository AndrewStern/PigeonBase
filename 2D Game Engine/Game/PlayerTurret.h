#pragma once
#include "engine\gameobject.h"

enum PlayerState
{
	Default,
	FirstPooAnimStart,
	FirstPooAnimPlay,
	FirstPooStatic,
	SecondPooAnimStart,
	SecondPooAnimPlay,
	SecondPooAStatic,
	ThirdPooAnimStart,
	ThirdPooAnimPlay,
	ThirdPooStatic
};

class PlayerTurret :public GameObject
{
	void Collided(GameObject*);
	void Start();
	void Update(unsigned long frameNumber);
	std::wstring GetType();
	public:

		//Total Player Life
		int _PlayerTurretLife;

		PlayerTurret() : GameObject(){}
		PlayerTurret(std::wstring objectName, Main* _Main, bool dynamic, bool physicsBody, Vector2 &_Position, LPCWSTR _TextureName, bool _IsSprite, int _Rows, int _Columns) : GameObject(objectName, _Main, dynamic, physicsBody, _Position, _TextureName, _IsSprite, _Rows, _Columns) {}

private:

	bool _PauseUpdate;
	PlayerState _PlayerState;
};