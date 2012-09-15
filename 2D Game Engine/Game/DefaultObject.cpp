#include "pch.h"
#include "Game\DefaultObject.h"

void DefaultObject::Collided(GameObject*){}

void DefaultObject::Start(){}

void DefaultObject::Update(unsigned long frameNumber){}

std::wstring DefaultObject::GetType()
{
	return L"DefaultObject";
}