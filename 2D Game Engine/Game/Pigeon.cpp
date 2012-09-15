#include "pch.h"
#include "Pigeon.h"

void Pigeon::Collided(GameObject* pBob)
{
	/*if(pBob->CompareTag(L"Bullet"))
	{
		this->Destroy();
		pBob->Destroy();
	}*/
	if(pBob->CompareTag(L"PlayerTurret"))
	{
		this->Destroy();
	}

}
void Pigeon::Start(){}
void Pigeon::Update(unsigned long frameNumber)
{
	
}
std::wstring Pigeon::GetType()
{
	return L"Pigeon";
}