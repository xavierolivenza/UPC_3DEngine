#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{

}

bool ParticleSystem::PreUpdate(float dt)
{
	return true;
}

bool ParticleSystem::Update(float dt)
{
	return true;
}

bool ParticleSystem::PostUpdate(float dt)
{
	return true;
}

bool ParticleSystem::CleanUp()
{
	return true;
}

bool ParticleSystem::CreateParticle()
{
	return true;
}

ParticleEmitter::EmitterShapeUnion::EmitterShapeUnion()
{

}