#include "physics.h"

Physics::Physics() {
	// Earth's gravity
	gravity.Set(0.0f, 9.80665f);
	world = nullptr;
	particlesWorld = nullptr;
	timeStepping = 1.0f / 60.0f;
	velocityIterations = 5;
	positionsIterations = 3;
	particleIterations = b2CalculateParticleIterations(gravity.y, 1.0f, timeStepping);
}

Physics::Physics(float gravityY, float gravityX, float timeStep, int32 velIteration, int32 posIteration, float particleRadius) {
	gravity.Set(gravityX, gravityY);
	world = nullptr;
	particlesWorld = nullptr;
	timeStepping = timeStep;
	velocityIterations = velIteration;
	positionsIterations = posIteration;
	particleIterations = b2CalculateParticleIterations(gravity.y, particleRadius, timeStepping);
}

void Physics::createWorld(b2Vec2 gravityGiven) {
	if(world != nullptr)
		return;
	gravity = gravityGiven;
	world = new b2World(gravityGiven);
}

void Physics::createParticles() {
	if(particlesWorld != nullptr || world == nullptr)
			return;
	const b2ParticleSystemDef particleSystemDef;
	particlesWorld = world->CreateParticleSystem(&particleSystemDef);
}

float32& Physics::getTimeStep() {
	return timeStepping;
}

int32& Physics::getVelocityIterations() {
	return velocityIterations;
}

int32& Physics::getPositionsIterations() {
	return positionsIterations;
}

int32& Physics::getParticleIterations() {
	return particleIterations;
}

void Physics::update() {
	// loop
	if(particlesWorld != nullptr) {
		world->Step(timeStepping, velocityIterations, positionsIterations, particleIterations);
		world->ClearForces();
	}else if(world != nullptr) {
		world->Step(timeStepping, velocityIterations, positionsIterations);
		world->ClearForces();
	}
}

b2World* Physics::getWorld() {
	return world;
}

b2ParticleSystem* Physics::getParticles() {
	return particlesWorld;
}
