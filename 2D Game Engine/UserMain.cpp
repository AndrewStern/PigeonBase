#include "pch.h"
#include "UserMain.h"
#include "Engine\Layer.h"

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
	_PigeonsLayer = AddLayer();

	switch(GameState) {
		case 0:
			// Background
			_background = new Background(L"Background", this, false, false, Vector2(0,0), L"Textures/background.png", false, 1, 1);
			_background->SetScale(2.0f, 2.0f);
			GetLayer(_BgLayer)->AddObjectToLayer(_background);

			// Start game button
			_button = new Btn(L"StartButton", this, false, true, Vector2(0, 0), L"start-button.png", false, 1, 1);
			GetLayer(_BtnLayer)->AddObjectToLayer(_button);
			break;

		case 1:
			// Background
			_background = new Background(L"Background", this, false, false, Vector2(0,0), L"Textures/background.png", false, 1, 1);
			_background->SetScale(2.0f, 2.0f);
			GetLayer(_BgLayer)->AddObjectToLayer(_background);

			// Ground
			_Ground1 = new Ground(L"Ground", this, false, false, Vector2(0,-5), L"Textures/Ground/concrete.png", false, 1, 1);
			_Ground1->SetScale(10.0f, 4.0f);
			GetLayer(_GrndLayer)->AddObjectToLayer(_Ground1);

			// Lamp Posts
			_lampPost1 = new LampPost(L"LampPost1", this, false, false, Vector2 (-3, -4), L"Textures/Lamp/lamp-front.png", false, 1, 1);
			_lampPost1->SetScale(0.5f, 0.5f);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost1);

			_lampPost2 = new LampPost(L"LampPost2", this, false, false, Vector2 (3, -4), L"Textures/Lamp/lamp-front.png", false, 1, 1);
            _lampPost2->SetScale(0.5f, 0.5f);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost2);

			_lampPost3 = new LampPost(L"LampPost3", this, false, false, Vector2 (-5,-6), L"Textures/Lamp/lamp-front.png", false, 1, 1);
			_lampPost3->SetScale(0.5f, 0.5f);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost3);

			_lampPost4 = new LampPost(L"LampPost4", this, false, false, Vector2 (5,-6), L"Textures/Lamp/lamp-front.png", false, 1, 1);
			_lampPost4->SetScale(0.5f, 0.5f);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost4);

			_lampPost5 = new LampPost(L"LampPost5", this, false, false, Vector2 (-8,-8), L"Textures/Lamp/lamp-front.png", false, 1, 1);
			_lampPost5->SetScale(0.5f, 0.5f);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost5);

			_lampPost6 = new LampPost(L"LampPost6", this, false, false, Vector2 (8,-8), L"Textures/Lamp/lamp-front.png", false, 1, 1);
			_lampPost6->SetScale(-0.5f, 0.5f);
			GetLayer(_EnvLayer)->AddObjectToLayer(_lampPost6);

			// Building
			_player = new Player( L"Player", this, false, true, Vector2(0,-2), L"Textures/palace.png", false, 1, 1);
			_player->SetGravity(0);
			_player->SetTag(L"Player");
			GetLayer(_PlyrLayer)->AddObjectToLayer(_player);
	
			// Score Text
			_text = new TextObject(L"Text", L"Thoma", 32, L"Score", this, Vector2(-9, 9));
			GetLayer(_EnvLayer)->AddObjectToLayer(_text);

			_score = new TextObject(L"Score", L"Thoma", 32, L"0", this, Vector2(-7, 9));
			GetLayer(_EnvLayer)->AddObjectToLayer(_score);

			// Leave Game Mode
			_button = new Btn(L"ExitButton", this, false, true, Vector2(8, -8), L"button.png", false, 1, 1);
			GetLayer(_BtnLayer)->AddObjectToLayer(_button);
			break;
	}
}

void UserMain::Clear()
{
	while(layerCount>0)
	{
		DeleteLayer(0);
		layerCount--;
	}
}

void UserMain::Update(unsigned long frameNumber)
{
	switch(GameState) {
		case 1:
			if(frameNumber % 60 == 0)
			{
				float X = -10 + (float) std::rand() / ((float) RAND_MAX / 20);
				float Y = 10;

				Pigeon * pigeon = new Pigeon(L"Pigeon", this, true, true, Vector2(X, Y), L"Textures/Pigeon/pigeon-idle.png", false, 1, 1);
				pigeon->SetTag(L"Pigeon");
				pigeon->SetScale(0.3f, 0.3f);

				// Add textures for animation
				pigeon->AddTexture(L"Textures/Pigeon/pigeon-sprite.png", true, 1, 11);

				// Start animation
				pigeon->SetTexture(L"Textures/Pigeon/pigeon-sprite.png");
				pigeon->SetAnimationSpeed(40.0f);

				pigeon->SetCollisionScale(0.2f, 0.2f);
				pigeon->SetGravity(1.0f);
				pigeon->RotateToLookAt(0, -6);
				pigeon->AddForce(0, 1000, Coordinate::Local);

				GetLayer(_PigeonsLayer)->AddObjectToLayer(pigeon);
			}

			break;
	}
}

void UserMain::OnPointerPressed(Vector2 _TouchPoint)
{
	// Get the game object at the touch point (click point)
	GameObject * _TouchObject = GetObjectAtPoint(_TouchPoint);

	// if its not a valid pointer theres nothing else to do
	if(_TouchObject == nullptr)
	{
		return;
	}

	switch(GameState) {
		case 0:
			if(_TouchObject->GetName() == L"StartButton")
			{
				// Lets start the game!
				GameState = 1;

				this->Clear();
				this->Start();
			}
			break;

		case 1:
			// Exit
			if(_TouchObject->GetName() == L"ExitButton")
			{
				// Lets call it a day!
				GameState = 0;

				this->Clear();
				this->Start();

				return;
			}

			if(_TouchObject->GetTag().compare(L"Pigeon") == 0)
			{
				_TouchObject->Destroy();

				Feathers * _f = new Feathers(L"Feathers", this, false, false, _TouchObject->position, L"Textures/Pigeon/pigeon-feather-idle.png", false, 1, 1);
				_f->AddTexture(L"Textures/Pigeon/pigeon-feather.png", true, 1, 16);

				_f->SetTexture(L"Textures/Pigeon/pigeon-feather.png");
				_f->SetAnimationSpeed(40.0f);

				GetLayer(_PigeonsLayer)->AddObjectToLayer(_f);

				_player->PlayerScore++;

				std::wostringstream ss;
				ss << _player->PlayerScore;

				_score->SetText(ss.str());
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
