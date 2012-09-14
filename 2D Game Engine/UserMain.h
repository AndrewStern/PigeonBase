#pragma once
#include "engine\main.h"
#include "game\PlayerTurret.h"
#include "game\Bullet.h"
#include "game\Pigeon.h"

using namespace _2D_Game_Engine;

class UserMain :
	public Main
{
public:

	void Start();
	void Update(unsigned long frameNumber);
	void OnPointerPressed(Vector2 _Point);
	void OnPointerMoved(Vector2 _Point);
	void OnPointerReleased(Vector2 _Point);
	void OnKeyDown(Windows::System::VirtualKey Key);
	void OnKeyUp(Windows::System::VirtualKey Key);
	PlayerTurret* _PlayerTurret;
	Bullet* _Bullet;
	
};

