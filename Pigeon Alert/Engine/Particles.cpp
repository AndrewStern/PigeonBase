#include "pch.h"
#include "Particles.h"


Particles::Particles(int _NumParticles, Vector2 pPosition, Vector2 pForce, float pLifespan, Texture pTexture)
{
	numParticles = _NumParticles;

	for(int i = 0; i< numParticles; ++i)
	{
		CreateParticle(i, pPosition, pForce, pLifespan, pTexture);
	}

}

Particles::~Particles(void)
{
}

void Particles::CreateParticle(int i, Vector2 pPosition, Vector2 pForce, float pLifespan, Texture pTexture)
{
	particles[i].position = Vector2(pPosition.x + (float)rand()/RAND_MAX, pPosition.y + (float)rand()/RAND_MAX);
	particles[i].force = pForce;
	particles[i].TimeRemaining = pLifespan;
	particles[i].particleTexture = pTexture;
	particles[i].isAlive = true;
}

void Particles::DestroyParticle(int _Index)
{

}

void Particles::Update()
{
	for(int i = 0; i < numParticles; ++i)
	{
		if((--particles[i].TimeRemaining) <= 0)
		{
			particles[i].isAlive = false;
		}
		particles[i].position += particles[i].force; 
	}
}

void Particles::Render()
{

}