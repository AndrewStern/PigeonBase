#pragma once


class GameContactListener : public b2ContactListener
{
public:
	GameContactListener(void);
	~GameContactListener(void);
	void BeginContact(b2Contact* contact);
};

