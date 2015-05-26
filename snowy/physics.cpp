#include "physics.h"
#include "object.h"
#include "hitTest.h"

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
	hitTest = new HitTest();
	world->SetContactListener(hitTest);
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
	hitTest = new HitTest();
	world->SetContactListener(hitTest);
}

Physics::Physics(const Physics &rhs) {
    gravity.Set(rhs.gravity.x, rhs.gravity.y);
	world = new b2World(gravity);
	// for fixed timestep, need to clear forces manually.
	world->SetAutoClearForces(false);
	particlesWorld = nullptr;
    if(rhs.particlesWorld != nullptr) {
        const b2ParticleSystemDef particleSystemDef;
        particlesWorld = world->CreateParticleSystem(&particleSystemDef);
    }
	timeStepping = rhs.timeStepping;
	velocityIterations = rhs.velocityIterations;
	positionsIterations = rhs.positionsIterations;
	particleIterations = rhs.particleIterations;
	mToPx = rhs.mToPx;
	hitTest = new HitTest();
	world->SetContactListener(hitTest);
}

Physics& Physics::operator=(const Physics &rhs) {
    if(this != &rhs) {
        gravity.Set(rhs.gravity.x, rhs.gravity.y);
        if(world != nullptr) {
            if(particlesWorld != nullptr) {
                world->DestroyParticleSystem(particlesWorld);
                particlesWorld = nullptr;
            }
            delete world;
        }
        world = new b2World(gravity);
        if(rhs.particlesWorld != nullptr) {
            const b2ParticleSystemDef particleSystemDef;
            particlesWorld = world->CreateParticleSystem(&particleSystemDef);
        }
        // for fixed timestep, need to clear forces manually.
        world->SetAutoClearForces(false);
        timeStepping = rhs.timeStepping;
        velocityIterations = rhs.velocityIterations;
        positionsIterations = rhs.positionsIterations;
        particleIterations = rhs.particleIterations;
        mToPx = rhs.mToPx;
        hitTest = new HitTest();
        world->SetContactListener(hitTest);
    }
    return *this;
}

Physics::~Physics() {
    if(particlesWorld != nullptr) {
        world->DestroyParticleSystem(particlesWorld);
        particlesWorld = nullptr;
    }
    delete world;
    delete hitTest;
    world = nullptr;
    hitTest = nullptr;
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
    /* //Extrapolation
    const float dt = accumulatorRatio * fixedTimeStep;
    for (b2Body* b = world->GetBodyList(); b != NULL; b = b->GetNext()) {
        if(b->GetType() == b2_staticBody || b->GetUserData() == nullptr)
            continue;
        int xTemp = mToPx * (b->GetPosition().x + dt * b->GetLinearVelocity().x);
        int yTemp = mToPx * (b->GetPosition().y + dt * b->GetLinearVelocity().y);
        static_cast<Object*>(b->GetUserData())->setPos(xTemp, yTemp);
        float rotDegrees = (b->GetAngle() + dt * b->GetAngularVelocity()) * (180.0f/3.141592f);
        static_cast<Object*>(b->GetUserData())->setRot(rotDegrees);
    } */
    //Interpolation
    const float oneMinusRatio = 1.0f - accumulatorRatio;
    for (b2Body* b = world->GetBodyList(); b != NULL; b = b->GetNext()) {
        if(b->GetType() == b2_staticBody || b->GetUserData() == nullptr)
            continue;

        float xTemp = mToPx * (accumulatorRatio * b->GetPosition().x)
                    + oneMinusRatio * (static_cast<Object*>(b->GetUserData())->getPrevPos().first);
        float yTemp = mToPx * (accumulatorRatio * b->GetPosition().y)
                    + oneMinusRatio * (static_cast<Object*>(b->GetUserData())->getPrevPos().second);

        float rotDegrees = (accumulatorRatio * b->GetAngle()) * TO_DEGREE
                            + oneMinusRatio * static_cast<Object*>(b->GetUserData())->getPrevRot();

        static_cast<Object*>(b->GetUserData())->setPos(xTemp, yTemp);
        static_cast<Object*>(b->GetUserData())->setRot(rotDegrees);
    }
}

void Physics::resetSmoothStates() {
    for (b2Body* b = world->GetBodyList(); b != NULL; b = b->GetNext()) {
        if(b->GetType() == b2_staticBody || b->GetUserData() == nullptr)
            continue;
        float xTemp = mToPx * (b->GetPosition().x);
        float yTemp = mToPx * (b->GetPosition().y);
        static_cast<Object*>(b->GetUserData())->setPos(xTemp, yTemp);
        float32 degree = b->GetAngle() * TO_DEGREE;
        static_cast<Object*>(b->GetUserData())->setRot(degree);
        //for interpolation
        static_cast<Object*>(b->GetUserData())->setPrevPos(xTemp, yTemp);
        static_cast<Object*>(b->GetUserData())->setPrevRot(degree);
    }
}

b2World* Physics::getWorld() {
	return world;
}

b2ParticleSystem* Physics::getParticles() {
	return particlesWorld;
}
