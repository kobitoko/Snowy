#include "physics.h"

Physics::Physics() {
	// Earth's gravity
	gravity.Set(0.0f, 9.80665f);
	world = nullptr;
	particlesWorld = nullptr;
	timeStep = 1.0f / 60.0f;
	velocityIterations = 5;
	positionsIterations = 3;
	particleIterations = b2CalculateParticleIterations(gravity.y, 1.0f, timeStep);
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
	return timeStep;
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
		world->Step(timeStep, velocityIterations, positionsIterations, particleIterations);
		world->ClearForces();
	}else if(world != nullptr) {
		world->Step(timeStep, velocityIterations, positionsIterations);
		world->ClearForces();
	}
}

Physics& Physics::get() {
	static Physics worldMaster;
	return worldMaster;
}

b2World* Physics::getWorld() {
	return world;
}

b2ParticleSystem* Physics::getParticles() {
	return particlesWorld;
}
