#include <iostream>

#include "Solver.h"

void SymplecticEuler::step(double dt, double time)
{
	for (auto& v : this->vessels)
	{
		// sum all gravitational acceleration due to bodies
		Vec3 grav_vector = Vec3();
		for (auto b : this->bodies)
		{
			grav_vector = grav_vector + b.getGravity(v.pos, false);
		}

		v.applyAccel(grav_vector);
		
	}

	for (auto& v : this->vessels)
	{
		v.vel = v.vel + v.accel * dt;
		v.pos = v.pos + v.vel * dt;
		v.clearAccels();
	}
}

void Yoshida8::step(double dt, double time)
{
	double w1 = 0.311790812418427e0;
	double w2 = -0.155946803821447e1;
	double w3 = -0.167896928259640e1;
	double w4 = 0.166335809963315e1;
	double w5 = -0.106458714789183e1;
	double w6 = 0.136934946416871e1;
	double w7 = 0.629030650210433e0;
	double w0 = (1 - 2 * (w1 + w2 + w3 + w4 + w5 + w6 + w7));

	double ds[] = { w7, w6, w5, w4, w3, w2, w1, w0, w1, w2, w3, w4, w5, w6, w7 };

	double cs[] = {w7 / 2, (w7 + w6) / 2, (w6 + w5) / 2, (w5 + w4) / 2,
		(w4 + w3) / 2, (w3 + w2) / 2, (w2 + w1) / 2, (w1 + w0) / 2,
		(w1 + w0) / 2, (w2 + w1) / 2, (w3 + w2) / 2, (w4 + w3) / 2,
		(w5 + w4) / 2, (w6 + w5) / 2, (w7 + w6) / 2, w7 / 2};

	for (int i = 0; i < 15; i++)
	{
		// -- update pos
		for (auto& b : this->bodies)
		{
			b.pos = b.pos + b.vel * cs[i] * dt;
		}
		for (auto& v : this->vessels)
		{
			v.pos = v.pos + v.vel * cs[i] * dt;
		}

		// -- compute accels
		for (auto& b : this->bodies)
		{
			// gravitational accel
			Vec3 grav_accel = Vec3();
			for (auto& b2 : this->bodies)
			{
				if (b.pos.x != b2.pos.x &&
					b.pos.y != b2.pos.y &&
					b.pos.z != b2.pos.z) // make this b != b2, the current check is a workaround
				{
					grav_accel = grav_accel + b2.getGravity(b.pos, false);
				}
			}
			b.applyAccel(grav_accel);
		}

		for (auto& v : this->vessels)
		{
			// gravitational accel
			Vec3 grav_accel = Vec3();
			for (auto& b : this->bodies)
			{
				grav_accel = grav_accel + b.getGravity(v.pos, false);
			}
			v.applyAccel(grav_accel);
		}

		// const. accel. mnv. accel
		for (auto& cam : this->const_accel_maneuvers)
		{
			cam.perform(time, this->bodies, this->vessels);
		}

		// -- update vel
		for (auto& b : this->bodies)
		{
			b.vel = b.vel + b.accel * ds[i] * dt;
			b.clearAccels();
		}
		for (auto& v : this->vessels)
		{
			v.vel = v.vel + v.accel * ds[i] * dt;
			v.clearAccels();
		}

	}

	// -- final position update
	for (auto& b : this->bodies)
	{
		b.pos = b.pos + b.vel * cs[15] * dt;
	}
	for (auto& v : this->vessels)
	{
		v.pos = v.pos + v.vel * cs[15] * dt;
	}

	// impulsive maneuvers
	for (auto& m : this->impulsive_maneuvers)
	{
		m.perform(time, this->bodies, this->vessels);
	}

	// done!
}