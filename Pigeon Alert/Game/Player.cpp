#include "pch.h"
#include "Player.h"

void Player::Collided(GameObject * _GameObject)
{
		
}

void Player::Start()
{
	PlayerScore = 0;
}

void Player::Update(unsigned long frameNumber)
{
	
}

std::wstring Player::GetType()
{
	return L"Player";
}