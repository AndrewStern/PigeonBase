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
	int x = (int) this->position.x;
	int y = (int) this->position.y;

	if(x > 40 || x < -40 || y > 40 || y < -40)
	{
		this->Destroy();
	}

	if(y < 4)
	{
		float X = -10 + (float) std::rand() / ((float) RAND_MAX / 20);

		this->SetGravity(-1.0f);
		this->RotateToLookAt(X, 20);
	}
}

std::wstring Pigeon::GetType()
{
	return L"Pigeon";
}