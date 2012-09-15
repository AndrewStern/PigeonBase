#include "pch.h"
#include "DirectXPage.g.h"
#include "UserMain.h"

using namespace Windows::UI::Xaml;

using namespace std;
#include <sstream>

UserMain::UserMain()
{
	// 0 = Start screen, 1 = In Game, 2 = Learderboard
	GameState = 0;
}

void UserMain::Start()
{
	// setup layers
	_BgLayer = AddLayer();
	_GrndLayer = AddLayer();
	_PlyrLayer = AddLayer();
	_EnvLayer = AddLayer();
	_BtnLayer = AddLayer();

	switch(GameState) {
		case 0:
			_background = new Background(L"Background", this, false, false, Vector2(0,0), L"bg.jpg", false, 1, 1);
			GetLayer(_BgLayer)->AddObjectToLayer(_background);
			_background->SetScale(2.0f, 2.0f);

			_text = new TextObject(L"Text", L"Thoma", 32, L"Pigeon Alert!", this, Vector2(-6, 6));
			GetLayer(_EnvLayer)->AddObjectToLayer(_text);
			
			_button = new Btn(L"Button", this, false, true, Vector2(0, 0), L"button.png", false, 1, 1);
			GetLayer(_BtnLayer)->AddObjectToLayer(_button);
			break;

		case 1:
			// Background
			_background = new Background(L"Background", this, false, false, Vector2(0,0), L"bg.jpg", false, 1, 1);
			GetLayer(_BgLayer)->AddObjectToLayer(_background);
			_background->SetScale(2.0f, 2.0f);

			// Ground
			_Ground1 = new Ground(L"Ground", this, false, false, Vector2(0,-5), L"grouundconcrete.png", false, 1, 1);
			GetLayer(_GrndLayer)->AddObjectToLayer(_Ground1);
			_Ground1->SetScale(10.0f, 4.0f);

			// Lamp Posts
			_lampPost1 = new LampPost(L"LampPost1", this, false, false, Vector2 (-3, -4), L"Lamp_Front.png", false, 1, 1);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost1);
			_lampPost1->SetScale(0.5f, 0.5f);

			_lampPost2 = new LampPost(L"LampPost2", this, false, false, Vector2 (3, -4), L"Lamp_Front.png", false, 1, 1);
            GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost2);
			_lampPost2->SetScale(0.5f, 0.5f);

			_lampPost3 = new LampPost(L"LampPost3", this, false, false, Vector2 (-5,-6), L"Lamp_Front.png", false, 1, 1);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost3);
			_lampPost3->SetScale(0.5f, 0.5f);

			_lampPost4 = new LampPost(L"LampPost4", this, false, false, Vector2 (5,-6), L"Lamp_Front.png", false, 1, 1);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost4);
			_lampPost4->SetScale(0.5f, 0.5f);

			_lampPost5 = new LampPost(L"LampPost5", this, false, false, Vector2 (-8,-8), L"Lamp_Side.png", false, 1, 1);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost5);
			_lampPost5->SetScale(0.5f, 0.5f);

			_lampPost6 = new LampPost(L"LampPost6", this, false, false, Vector2 (8,-8), L"Lamp_Side.png", false, 1, 1);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost6);
			_lampPost6->SetScale(-0.5f, 0.5f);

			// Building
			_player = new Player( L"Player", this, false, true, Vector2(0,-2), L"palace.png", false, 1, 1);
			GetLayer(_PlyrLayer)->AddObjectToLayer(_player);
			_player->SetGravity(0);
			_player->SetTag(L"Player");
	
			// Score Text
			_text = new TextObject(L"Text", L"Thoma", 32, L"Score", this, Vector2(-6, 6));
			GetLayer(_EnvLayer)->AddObjectToLayer(_text);

			_score = new TextObject(L"Score", L"Thoma", 32, L"0", this, Vector2(-4, 6));
			GetLayer(_EnvLayer)->AddObjectToLayer(_score);

			// Leave Game Mode
			_button = new Btn(L"Button", this, false, true, Vector2(8, -8), L"button.png", false, 1, 1);
			GetLayer(_BtnLayer)->AddObjectToLayer(_button);
			break;
	}
}

void UserMain::ClearStart()
{
	DeleteLayer(_BgLayer);
	DeleteLayer(_PlyrLayer);
	DeleteLayer(_EnvLayer);
	DeleteLayer(_BtnLayer);
	DeleteLayer(_GrndLayer);
/*
	delete _background;
	delete _button;
	delete _text;
*/
}

void UserMain::ClearGame()
{
	// Clear Collection
	Pigeons.clear();

	DeleteLayer(_BgLayer);
	DeleteLayer(_PlyrLayer);
	DeleteLayer(_EnvLayer);
	DeleteLayer(_BtnLayer);
	DeleteLayer(_GrndLayer);
/*
	delete _background;
	delete _Ground1;

	delete _lampPost1;
	delete _lampPost2;
	delete _lampPost3;
	delete _lampPost4;
	delete _lampPost5;
	delete _lampPost6;

	delete _player;

	delete _text;
	delete _score;
*/
}

void UserMain::Update(unsigned long frameNumber)
{
	switch(GameState) {
		case 1:
			if(frameNumber % 60 == 0)
			{
				float X = -10 + (float) std::rand() / ((float) RAND_MAX / 20);
				float Y = 10;

				Pigeon * pidgeon = new Pigeon(L"Pigeon", this, true, true, Vector2(X, Y), L"pigeon.png", false, 1, 1);
				pidgeon->SetTag(L"Pigeon");
		
				pidgeon->SetCollisionScale(1.0f, 1.0f);

				pidgeon->SetGravity(0);
				pidgeon->RotateToLookAt(0, -4);
				pidgeon->AddForce(0, 800, Coordinate::Local);

				GetLayer(_EnvLayer)->AddObjectToLayer(pidgeon);

				Pigeons.push_back(pidgeon);
			}
			break;
	}
}

void UserMain::OnPointerPressed(Vector2 _TouchPoint)
{
	GameObject * _TouchObj = GetObjectAtPoint(_TouchPoint);

	// if its not a valid pointer theres nothing else to do
	if(_TouchObj == nullptr)
	{
		return;
	}

	switch(GameState) {
		case 0:
			if(_TouchObj == _button)
			{
				// Lets start the game!
				GameState = 1;

				this->ClearStart();
				this->Start();
			}
			break;

		case 1:
			// Exit
			if(_TouchObj == _button)
			{
				// Lets call it a day!
				GameState = 0;

				this->ClearGame();
				this->Start();

				return;
			}

			for(int i = 0; i < (int) Pigeons.size(); i++)
			{
				Pigeon * pigeon = Pigeons.at(i);

				if(_TouchObj == pigeon)
				{
					_TouchObj->Destroy();
					_player->PlayerScore++;

					std::wostringstream ss;
					ss << _player->PlayerScore;

					_score->SetText(ss.str());

					return;
				}
			}
			break;
	}
}

void UserMain::OnPointerMoved(Vector2 _TouchPoint)
{

}

void UserMain::OnPointerReleased(Vector2 _TouchPoint)
{
	
}

void UserMain::OnKeyDown(Windows::System::VirtualKey Key)
{
	
}

void UserMain::OnKeyUp(Windows::System::VirtualKey Key)
{

}
