#include "pch.h"
#include "Feathers.h"

void Feathers::Collided(GameObject*)
{

}

void Feathers::Start()
{

}

void Feathers::Update(unsigned long frameNumber)
{
	if(this->GetCurrentTextureFrame() == 15)
	{
		this->Destroy();
	}
}

std::wstring Feathers::GetType()
{
	return L"Feathers";
}