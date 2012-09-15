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
	_EnvLayer = AddLayer();
	_BtnLayer = AddLayer();

	switch(GameState) {
		case 0:
			_background = new Background(L"Background", this, false, false, Vector2(0,0), L"bg.jpg", false, 1, 1);
			_background->SetScale(2.0f, 2.0f);
			GetLayer(_EnvLayer)->AddObjectToLayer(_background);

			_text = new TextObject(L"Text", L"Thoma", 32, L"Pigeon Alert!", this, Vector2(-9, 9));
			GetLayer(_EnvLayer)->AddObjectToLayer(_text);
			
			_button = new Btn(L"Button", this, false, true, Vector2(0, 0), L"button.png", false, 1, 1);
			GetLayer(_BtnLayer)->AddObjectToLayer(_button);
			break;

		case 1:
			_background = new Background(L"Background", this, false, false, Vector2(0,0), L"bg.jpg", false, 1, 1);
			_background->SetScale(2.0f, 2.0f);
			GetLayer(_EnvLayer)->AddObjectToLayer(_background);

			_player = new Player( L"Player", this, false, false, Vector2(0,-7), L"palace.png", false, 1, 1);
			GetLayer(_EnvLayer)->AddObjectToLayer(_player);
	
			_text = new TextObject(L"Text", L"Thoma", 32, L"Score", this, Vector2(-9, 9));
			GetLayer(_EnvLayer)->AddObjectToLayer(_text);

			_score = new TextObject(L"Score", L"Thoma", 32, L"0", this, Vector2(-7, 9));
			GetLayer(_EnvLayer)->AddObjectToLayer(_score);
			break;
	}
}

void UserMain::Clear()
{
	DeleteLayer(_EnvLayer);
	DeleteLayer(_BtnLayer);
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
		
				pidgeon->SetCollisionScale(1.0f, 1.0f);

				pidgeon->SetGravity(0);
				pidgeon->RotateToLookAt(0, -7);
				pidgeon->AddForce(0, 1000, Coordinate::Local);

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
				this->Clear();
				this->Start();
			}
			break;

		case 1:
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
