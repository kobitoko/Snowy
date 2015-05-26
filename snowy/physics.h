#pragma once
#include <Box2D/Box2D.h>

class HitTest;

class Physics {
public:
    /// stores the HitTest object.
    HitTest* hitTest;

    /// create a Physics object
    Physics();
    Physics(float gravityY, float gravityX = 0.0f, float metricToPx = 10.0f, float timeStep = 1.0f/60.0f, int32 velIteration = 5, int32 posIteration = 3, float particleRadius = 1.0f);
    Physics(const Physics &rhs);
    Physics& operator=(const Physics &rhs);
    ~Physics();

	/// Creates only once a physics world with given gravity. Does nothing upon multiple calls.
	void createWorld(b2Vec2 gravityGiven);

	/// Creates only once a particle system within the world. Does nothing upon multiple calls.
	/// createWorld(float) has to be called first, else this function will do nothing.
	void createParticles();

	/// gets access to the update step variable VelocityIterations. Default value is 5.
	int32& getVelocityIterations();

	/// gets access to the update step variable PositionsIterations. Default value is 3.
	int32& getPositionsIterations();

	/// gets access to the update step variable ParticleIterations.
	/// Default value is b2CalculateParticleIterations(gravity.y, 1.0f, TimeStep);
	int32& getParticleIterations();

	/// Updates all the physics to the next step iterations.
	/// Does nothing if createWorld(b2Vec2) was not called before.
	/// Also updates the particles if createParticles() was called before.
	void update();

    /// Interpolation smoothing.
    /// From http://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
    void smoothStates(float accumulatorRatio, const float fixedTimeStep);
    void resetSmoothStates();

	/// returns the world so you can modify it.
	b2World* getWorld();

	/// returns the particles so you can modify it.
	b2ParticleSystem* getParticles();

private:
	b2Vec2 gravity;
	b2World* world;
	b2ParticleSystem* particlesWorld;
	//= 1.0f / 1000.0f;
	float32 timeStepping;
	// recommended iterations is 8 total. 5 velocity and 3 position. b2CalculateParticleIterations(-9.80665f, 0.8f, timeStep)
	int32 velocityIterations;
	int32 positionsIterations;
	int32 particleIterations;
	// metric units to pixel scale
    float mToPx;
    // conversion radians to degree
    const float TO_DEGREE = 180.0f/3.141592f;

};
