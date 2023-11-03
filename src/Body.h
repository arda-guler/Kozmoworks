#pragma once
#include "constants.h"
#include "RigidBody.h"

class Body : public RigidBody
{
public:
	double Rmin, Rmax;
	double mu;
	
	// this invokes the constructor of the RigidBody class too
	Body()
	{
		pos = Vec3();
		vel = Vec3();
		accel = Vec3();

		orient = Mtx3x3();
		ang_vel = Vec3();
		ang_accel = Vec3();

		moment_of_inertia = Mtx3x3(9.736e37, 0, 0,
			0, 9.736e37, 0,
			0, 0, 9.736e37);

		mass = 5.9722e24;

		Rmin = 0;
		Rmax = 0;

		// calculate this here once so you don't have to make an unnecessary 
		// multiplication every time getGravity() is called
		mu = mass * Constants::G; 
	}

	Body(int pid, Vec3 ppos, Vec3 pvel, Vec3 paccel,
		Mtx3x3 porient, Vec3 pang_vel, Vec3 pang_accel,
		double pmass, Mtx3x3 pmoment_of_inertia,
		double pRmin, double pRmax)
	{
		id = pid;

		pos = ppos;
		vel = pvel;
		accel = paccel;

		orient = porient;
		ang_vel = pang_vel;
		ang_accel = pang_accel;

		mass = pmass;
		moment_of_inertia = pmoment_of_inertia;

		Rmin = pRmin;
		Rmax = pRmax;

		// calculate this here once so you don't have to make an unnecessary 
		// multiplication every time getGravity() is called
		mu = mass * Constants::G;
	}

	Vec3 getGravity(Vec3 tpos, bool harmonics);
	Vec3 getTangentialVel(Vec3 tpos);
};