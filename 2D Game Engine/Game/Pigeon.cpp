#include "pch.h"
#include "Pigeon.h"

void Pigeon::Collided(GameObject * _GameObject)
{
	
}

void Pigeon::Start()
{
	RotateTextureWithPhysics = false;
}

void Pigeon::Update(unsigned long frameNumber)
{
	int x = (int) this->GetVelocity().x;
	int y = (int) this->GetVelocity().y;

	if(x > 40 || x < -40 || y > 40 || y < -40)
	{
		this->Destroy();
	}

	if(y < 0)
	{
		// we're here!
	}
}

std::wstring Pigeon::GetType()
{
	return L"Pigeon";
}