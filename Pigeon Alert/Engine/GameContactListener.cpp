#include "pch.h"
#include "GameObject.h"
#include "GameContactListener.h"

GameContactListener::GameContactListener(void)
{	
}


GameContactListener::~GameContactListener(void)
{
}

void GameContactListener::BeginContact(b2Contact* contact)
{
	GameObject* object1;
	GameObject* object2; 
	object1 = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	object2 = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData();
	if(object1 != nullptr)
	{
		//object->collisionBox->ApplyForceToCenter(b2Vec2(0, 15000));
		object1->Collided(object2);
		return;
	}
	if(object2 != nullptr)
	{
		//object->collisionBox->ApplyForceToCenter(b2Vec2(0, 15000));
		object2->Collided(object1);
		return;
	}
} 