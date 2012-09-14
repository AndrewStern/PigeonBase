#include "pch.h"
#include "Bullet.h"

void Bullet::Collided(GameObject* bBob)
{
	if(bBob->CompareTag(L"Pigeon"))
	{
		this->Destroy();
		bBob->Destroy();
	}
	
}
void Bullet::Start(){}
void Bullet::Update(unsigned long frameNumber)
{
	
}
std::wstring Bullet::GetType()
{
	return L"Bullet";
}