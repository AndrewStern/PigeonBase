#include "pch.h"
#include "ExampleTextObject.h"

void ExampleTextObject::Collided(GameObject*){}

void ExampleTextObject::Start(){}

void ExampleTextObject::Update(unsigned long frameNumber){}

std::wstring ExampleTextObject::GetType()
{
	return L"ExampleTextObject";
}