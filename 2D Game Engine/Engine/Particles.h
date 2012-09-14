#pragma once
#include"Texture.h"

struct Particle
{
	Vector2 position;
	Vector2 force;
	float TimeRemaining;
	Texture particleTexture;
	bool isAlive;
};

class Particles
{
public:
	Particles(int, Vector2, Vector2, float, Texture);
	~Particles(void);

private:
	
	void CreateParticle(int i, Vector2 pPosition, Vector2 pForce, float pLifespan, Texture pTexture);
	void DestroyParticle(int);

	void Update();
	void Render();

	int numParticles;
	Particle particles[1000];
};

