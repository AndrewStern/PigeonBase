#pragma once
#include "engine\main.h"

#include "Game\Background.h"
#include "Game\Player.h"
#include "Game\Pigeon.h"
#include "Game\TextObject.h"
#include "Game\Btn.h"

using namespace _Pigeon_Alert;

class UserMain : public Main
{
public:
	UserMain();
	void Start();
	void Update(unsigned long frameNumber);
	void OnPointerPressed(Vector2 _Point);
	void OnPointerMoved(Vector2 _Point);
	void OnPointerReleased(Vector2 _Point);
	void OnKeyDown(Windows::System::VirtualKey Key);
	void OnKeyUp(Windows::System::VirtualKey Key);
	void Clear();
	
	Background * _background;
	Player * _player;
	Btn * _button;

	// Collection of game objects
	vector<Pigeon *> Pigeons;

	// Layers
	int _EnvLayer;
	int _BtnLayer;

	// Text
	TextObject * _text;
	TextObject * _score;

	// game state
	int GameState;
};

