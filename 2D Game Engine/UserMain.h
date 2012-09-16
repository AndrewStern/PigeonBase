#pragma once
#include "engine\main.h"

#include "Game\Background.h"
#include "Game\Player.h"
#include "Game\Pigeon.h"
#include "Game\TextObject.h"
#include "Game\Btn.h"
#include "Game\Ground.h"
#include "Game\LampPost.h"
#include "Game\PooBomb.h"
#include "Game\Feathers.h"

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
	
	// Game objects
	Background * _background;
	Player * _player;
	Btn * _button;
	Ground * _Ground1;

	LampPost * _lampPost1;
	LampPost * _lampPost2;
	LampPost * _lampPost3;
	LampPost * _lampPost4;
	LampPost * _lampPost5;
	LampPost * _lampPost6;

	// Text
	TextObject * _text;
	TextObject * _score;
	TextObject * _time;
	TextObject * _timerlabel;

	TextObject * _HighScore1;
	TextObject * _HighScore2;
	TextObject * _HighScore3;
	TextObject * _HighScore4;
	TextObject * _HighScore5;

	TextObject * _HighScoreNames1;
	TextObject * _HighScoreNames2;
	TextObject * _HighScoreNames3;
	TextObject * _HighScoreNames4;
	TextObject * _HighScoreNames5;

	// Layers
	int _BgLayer;
	int _GrndLayer;
	int _EnvLayer;
	int _BtnLayer;
	int _PlyrLayer;
	int _PigeonsLayer;

	// game state
	int GameState;
	int GameTime;
};