#pragma once
#include "engine\main.h"

#include "Game\Background.h"
#include "Game\Player.h"
#include "Game\Pigeon.h"
#include "Game\TextObject.h"
#include "Game\Btn.h"
#include "Game\Ground.h"
#include "Game\LampPost.h"

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
	void ClearGame();
	void ClearStart();
	
	Background * _background;
	Player * _player;

	Btn * _button;

	Ground * _Ground1;

	LampPost* _lampPost1;
	LampPost* _lampPost2;
	LampPost* _lampPost3;
	LampPost* _lampPost4;
	LampPost* _lampPost5;
	LampPost* _lampPost6;

	// Collection of game objects
	vector<Pigeon *> Pigeons;

	// Layers
	int _BgLayer;
	int _GrndLayer;
	int _EnvLayer;
	int _BtnLayer;
	int _PlyrLayer;

	// Text
	TextObject * _text;
	TextObject * _score;

	// game state
	int GameState;
};

