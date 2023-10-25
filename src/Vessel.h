#pragma once
#include "RigidBody.h"

class Vessel : public RigidBody
{
public:
	double prop_mass;

	Vessel()
	{
		prop_mass = 0;
	}

	Vessel(Vec3 ppos, Vec3 pvel, Vec3 paccel,
		Mtx3x3 porient, Vec3 pang_vel, Vec3 pang_accel,
		double pmass, Mtx3x3 pmoment_of_inertia, double pprop_mass)
	{
		pos = ppos;
		vel = pvel;
		accel = paccel;
		
		orient = porient;
		ang_vel = pang_vel;
		ang_accel = pang_accel;

		mass = pmass;
		moment_of_inertia = pmoment_of_inertia;
		prop_mass = pprop_mass;
	}
};