#include "pch.h"
#include "DirectXPage.g.h"
#include "Game\Background.h"
#include "UserMain.h"


using namespace Windows::UI::Xaml;


void UserMain::Start()
{
    AddLayer();
	AddLayer();
	AddLayer();

	Background * background = new Background(L"Background", this, false, false, Vector2(0,0), L"bg.jpg", false, 1, 1);
	background->SetDrawScale(2, 2);
	GetLayer(0)->AddObjectToLayer(background);

	PlayerTurret* _PlayerTurret = new PlayerTurret( L"PlayerTurret", this, false, false, Vector2(0,0), L"Eblock1.png", false, 1, 1);
	GetLayer(0)->AddObjectToLayer(_PlayerTurret);
	_PlayerTurret->SetScale(1,1);
	_PlayerTurret->SetTag(L"PlayerTurret");
}

void UserMain::Update(unsigned long frameNumber)
{
	if(frameNumber%100 == 0)
	{
		/*float MIN_X = -12;
		float MIN_Y = -12;
		float MAX_X =  12;
		float MAX_Y =  12;*/
                           
		float X = -15 + (float) std::rand()/ ((float)RAND_MAX/(40));
		float Y = -15 + (float) std::rand()/ ((float)RAND_MAX/(40));

		// LEFT //
		Pigeon* _Pigeon = new Pigeon(L"Pigeon", this, true,true, Vector2(X,Y), L"EBlock2.png",false,1,1);
		_Pigeon->SetTag(L"Pigeon");
		_Pigeon->SetCollisionScale(0.75f,0.75f);
		_Pigeon->SetScale(0.75,0.75);
		_Pigeon->SetGravity(0);
		_Pigeon->RotateToLookAt(0,0);
		_Pigeon->AddForce(0, 250,Coordinate::Local);
		GetLayer(1)->AddObjectToLayer(_Pigeon);
		

		//RIGHT//
		/*GameObject* Pigeon2 = new GameObject(L"Pigeon", this, false,false, Vector2(X,0), L"EBlock2.png",false,1,1);
		Pigeon2->SetTag(L"Pigeon2");
		Pigeon2->SetScale(0.75,0.75);
		Pigeon2->SetGravity(0);
		Pigeon2->RotateToLookAt(0,0);
		Pigeon2->AddForce(0, 250,Coordinate::Local);
		GetLayer(1)->AddObjectToLayer(Pigeon2);
		*/
	}
	
}

void UserMain::OnPointerPressed(Vector2 _TouchPoint)
{

	Bullet* _Bullet = new Bullet (L"Bullet", this, true, true, Vector2(0,0), L"Eblock3.png", false, 1, 1);
	GetLayer(1)->AddObjectToLayer( _Bullet );
	_Bullet->SetTag(L"Bullet");
	_Bullet->SetGravity(0);
	_Bullet->SetScale(0.5,0.5);
	_Bullet->RotateToLookAt(_TouchPoint);
	_Bullet->AddForce(0,600,Coordinate::Local);
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
