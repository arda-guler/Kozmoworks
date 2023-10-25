#include "RigidBody.h"

void RigidBody::applyAccel(Vec3 accel)
{
	this->accel = this->accel + accel;
}

void RigidBody::applyForce(Vec3 force)
{
	this->accel = this->accel + force / this->mass;
}

void RigidBody::applyAngAccel(Vec3 ang_accel)
{
	this->ang_accel = this->ang_accel + ang_accel;
}

void RigidBody::applyTorque(Vec3 torque)
{
	// rotate moment of inertia matrix according to the object's orientation matrix in the global space
	Vec3 MoI_oriented_x = Vec3(this->orient.m11 * this->moment_of_inertia.m11, this->orient.m12 * this->moment_of_inertia.m11, this->orient.m13 * this->moment_of_inertia.m11);
	Vec3 MoI_oriented_y = Vec3(this->orient.m21 * this->moment_of_inertia.m22, this->orient.m22 * this->moment_of_inertia.m22, this->orient.m23 * this->moment_of_inertia.m22);
	Vec3 MoI_oriented_z = Vec3(this->orient.m31 * this->moment_of_inertia.m33, this->orient.m32 * this->moment_of_inertia.m33, this->orient.m33 * this->moment_of_inertia.m33);

	Mtx3x3 moment_of_inertia_oriented = Mtx3x3(MoI_oriented_x, MoI_oriented_y, MoI_oriented_z);
	this->ang_accel = this->ang_accel + moment_of_inertia_oriented.inverse().dot(torque);
}

void RigidBody::clearAccels()
{
	this->accel = Vec3(0, 0, 0);
	this->ang_accel = Vec3(0, 0, 0);
}

void RigidBody::updateMass(double delta_mass)
{
	this->mass = this->mass + delta_mass;
}
