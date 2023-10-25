#pragma once

#include "Vector3.h"
#include "Matrix3x3.h"

class RigidBody
{
public:
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;

	Mtx3x3 orient;
	Vec3 ang_vel;
	Vec3 ang_accel;

	double mass;
	Mtx3x3 moment_of_inertia;

	RigidBody()
	{
		pos = Vec3();
		vel = Vec3();
		accel = Vec3();

		orient = Mtx3x3();
		ang_vel = Vec3();
		ang_accel = Vec3();

		mass = 1;
		moment_of_inertia = Mtx3x3();
	}

	RigidBody(Vec3 ppos, Vec3 pvel, Vec3 paccel,
		Mtx3x3 porient, Vec3 pang_vel, Vec3 pang_accel,
		double pmass, Mtx3x3 pmoment_of_inertia)
	{
		pos = ppos;
		vel = pvel;
		accel = paccel;

		orient = porient;
		ang_vel = pang_vel;
		ang_accel = pang_accel;

		mass = pmass;
		moment_of_inertia = pmoment_of_inertia;
	}

	void updateMass(double delta_mass);
	void applyForce(Vec3 force);
	void applyAccel(Vec3 accel);
	void applyTorque(Vec3 torque);
	void applyAngAccel(Vec3 ang_accel);
	void clearAccels();
};