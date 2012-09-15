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
void PlayerTurret::Start(){}
void PlayerTurret::Update(unsigned long frameNumber)
{
	
}
std::wstring PlayerTurret::GetType()
{
	return L"PlayerTurret";
}
