#include <iostream>

#include "Atmosphere.h"

void PolyAtmo::apply()
{
	double R = (this->target->pos - this->parent->pos).mag();

	// target too far away - no aero drag
	if (R > this->max_R) 
	{
		return;
	}

	// TODO: once you have a "radius-at-latitude" func, use that result instead of Rmin here
	double alt = R - this->parent->Rmin;

	
	// the velocity of airflow relative to global frame = 
	// velocity of the parent body in global frame + velocity of atmosphere relative to parent body
	Vec3 atmo_vel = this->parent->vel + this->parent->getTangentialVel(this->target->pos);

	Vec3 vel = this->target->vel - atmo_vel;
	double vel_mag = vel.mag();
	Vec3 drag_dir = -vel.normalized();

	double density = 0;
	for (int i = 0; i < this->coeffs.size(); i++)
	{
		density += this->coeffs[this->coeffs.size() - 1 - i] * pow(alt, i);
	}

	// drag_multipler = 0.5 * Cd * A
	double drag_amount = this->drag_multiplier * density * vel_mag * vel_mag;
	Vec3 drag_force = drag_dir * drag_amount;
	Vec3 drag_accel = drag_force / this->target->mass;

	this->target->applyAccel(drag_accel);
}

void ExpoAtmo::apply()
{
	double R = (this->target->pos - this->parent->pos).mag();

	// target too far away - no aero drag
	if (R > this->max_R) 
	{
		return;
	}

	// TODO: once you have a "radius-at-latitude" func, use that result instead of Rmin here
	double alt = R - this->parent->Rmin;

	// the velocity of airflow relative to global frame = 
	// velocity of the parent body in global frame + velocity of atmosphere relative to parent body
	Vec3 atmo_vel = this->parent->vel + this->parent->getTangentialVel(this->target->pos);

	Vec3 vel = this->target->vel - atmo_vel;
	double vel_mag = vel.mag();
	Vec3 drag_dir = -vel.normalized();

	double density = this->base_density * exp(-alt / scale_height);

	// drag_multipler = 0.5 * Cd * A
	double drag_amount = this->drag_multiplier * density * vel_mag * vel_mag;
	Vec3 drag_force = drag_dir * drag_amount;
	Vec3 drag_accel = drag_force / this->target->mass;

	this->target->applyAccel(drag_accel);
}
