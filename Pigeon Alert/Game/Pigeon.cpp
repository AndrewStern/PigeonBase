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
	if(this->GetVelocity().x > 40 || this->GetVelocity().x < -40 || this->GetVelocity().y > 40 || this->GetVelocity().y < -40)
	{
		this->Destroy();
	}
}

std::wstring Pigeon::GetType()
{
	return L"Pigeon";
}