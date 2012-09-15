#include "pch.h"
#include "PlayerTurret.h"

void PlayerTurret::Collided(GameObject* pBob)
{
	if(pBob->CompareTag(L"Pigeon"))
	{
		this->Destroy();
		pBob->Destroy();
	}
}
void PlayerTurret::Start()
{
	_PlayerState = PlayerState::Default;
	// Max House Life 
	_PlayerTurretLife = 3;
	_PauseUpdate = false;
}
void PlayerTurret::Update(unsigned long frameNumber)
{
if(_PauseUpdate)
		return;
/////////////////////////////////////////House POO Stage//////////////////////////////////////
	if(_PlayerState == PlayerState::FirstPooAnimStart)
	{
		// Setting the texture 
		this->SetTexture(L"FirstHit.png");
		// Set the next stage 
		_PlayerState = PlayerState::FirstPooAnimPlay;
		// Decrease house life 
		this->_PlayerTurretLife =2;
	}

	if(_PlayerState == PlayerState::FirstPooAnimPlay)
	{
		// Wait untill the texture frame reach last one to chage to next stage 
		if(this->GetCurrentTextureFrame() == 15)
		{
			_PlayerState = PlayerState::FirstPooStatic;
		}
	}

	if(_PlayerState == PlayerState::FirstPooStatic)
	{
		// Setting the texture 
		this->SetTexture(L"NormalPoo.png");
	}
/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////House POO Stage//////////////////////////////////////
	if(_PlayerState == PlayerState::SecondPooAnimStart)
	{
		this->_PlayerTurretLife = 1;
		this->SetTexture(L"SecondHit.png");
		_PlayerState = PlayerState::SecondPooAnimPlay;
	}

	if(_PlayerState == PlayerState::SecondPooAnimPlay)
	{
		if(this->GetCurrentTextureFrame() == 15)
		{
			_PlayerState = PlayerState::SecondPooAStatic;
		}
	}

	if(_PlayerState == PlayerState::SecondPooAStatic)
	{
		
		float colScaleX = this->GetCollisionScale().x;
		float colScaleY = this->GetCollisionScale().y ;

		this->SetCollisionScale(colScaleX,  (colScaleY - (colScaleY/2)));
		this->SetTexture(L"HalfPoo.png");
	}
////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////House POO Stage//////////////////////////////////////
	if(_PlayerState == PlayerState::ThirdPooAnimStart)
	{   
		this->_PlayerTurretLife =0;
		this->SetTexture(L"ThirdHit.png");
		_PlayerState = PlayerState::ThirdPooAnimPlay;
	}

	if(_PlayerState == PlayerState::ThirdPooAnimPlay)
	{
		if(this->GetCurrentTextureFrame() == 15)
		{
			_PlayerState = PlayerState::ThirdPooStatic;
		}
	}

	if(_PlayerState == PlayerState::ThirdPooStatic)
	{
		this->SetTexture(L"FullPoo.png");
		this->SetPhysicsBody(false);
		this->_PlayerTurretLife = 0;
		_PauseUpdate = true;
	}
	
}
std::wstring PlayerTurret::GetType()
{
	return L"PlayerTurret";
}
