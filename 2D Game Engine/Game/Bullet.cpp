#include "pch.h"
#include "Bullet.h"

void Bullet::Collided(GameObject* pBob)
{
	if(pBob->CompareTag(L"Pigeon"))
	{
		this->Destroy();
		pBob->Destroy();
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