#include "physics.h"
#include "object.h"

Physics::Physics() {
	// Earth's gravity
	gravity.Set(0.0f, 9.80665f);
	mToPx = 10.0f;
	world = new b2World(gravity);
	// for fixed timestep, need to clear forces manually.
	world->SetAutoClearForces(false);
	particlesWorld = nullptr;
	timeStepping = 1.0f / 60.0f;
	velocityIterations = 5;
	positionsIterations = 3;
	particleIterations = b2CalculateParticleIterations(gravity.y, 1.0f, timeStepping);
}

Physics::Physics(float gravityY, float gravityX, float metricToPx, float timeStep, int32 velIteration, int32 posIteration, float particleRadius) {
	gravity.Set(gravityX, gravityY);
	world = new b2World(gravity);
	// for fixed timestep, need to clear forces manually.
	world->SetAutoClearForces(false);
	particlesWorld = nullptr;
	timeStepping = timeStep;
	velocityIterations = velIteration;
	positionsIterations = posIteration;
	particleIterations = b2CalculateParticleIterations(gravity.y, particleRadius, timeStepping);
	mToPx = metricToPx;
}

void Physics::createParticles() {
	if(particlesWorld != nullptr || world == nullptr)
			return;
	const b2ParticleSystemDef particleSystemDef;
	particlesWorld = world->CreateParticleSystem(&particleSystemDef);
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
	}else if(world != nullptr) {
		world->Step(timeStepping, velocityIterations, positionsIterations);
	}
}

void Physics::smoothStates(float accumulatorRatio, const float fixedTimeStep) {
    const float dt = accumulatorRatio * fixedTimeStep;
    for (b2Body* b = world->GetBodyList(); b != NULL; b = b->GetNext()) {
        if(b->GetType() == b2_staticBody || b->GetUserData() == nullptr)
            continue;
        int xTemp = mToPx * (b->GetPosition().x + dt * b->GetLinearVelocity().x);
        int yTemp = mToPx * (b->GetPosition().y + dt * b->GetLinearVelocity().y);
        static_cast<Object*>(b->GetUserData())->setPos(xTemp, yTemp);
        float rotDegrees = (b->GetAngle() + dt * b->GetAngularVelocity()) * (180.0f/3.141592f);
        static_cast<Object*>(b->GetUserData())->setRot(rotDegrees);
    }
}

void Physics::resetSmoothStates() {
    for (b2Body* b = world->GetBodyList(); b != NULL; b = b->GetNext()) {
        if(b->GetType() == b2_staticBody || b->GetUserData() == nullptr)
            continue;
        int xTemp = mToPx * (b->GetPosition().x);
        int yTemp = mToPx * (b->GetPosition().y);
        static_cast<Object*>(b->GetUserData())->setPos(xTemp, yTemp);
        static_cast<Object*>(b->GetUserData())->setRot(b->GetAngle() * (180.0f/3.141592f));
    }
}

b2World* Physics::getWorld() {
	return world;
}

b2ParticleSystem* Physics::getParticles() {
	return particlesWorld;
}
