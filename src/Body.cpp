#include "Body.h"
#include "constants.h"

Vec3 Body::getGravity(Vec3 tpos, bool harmonics=false)
{
	if (!harmonics) {
		Vec3 grav_dir = (this->pos - tpos).normalized();
		double dist = (tpos - this->pos).mag();
		double grav_mag = this->mu / (dist * dist);
		return grav_dir * grav_mag;
	}
	else
	{
		// TODO: Spherical harmonics
		// right now, this is a duplicate of the point-mass
		// version, just to keep the compiler happy
		Vec3 grav_dir = (this->pos - tpos).normalized();
		double dist = (tpos - this->pos).mag();
		double grav_mag = this->mu / (dist * dist);
		return grav_dir * grav_mag;
	}
}

// get the linear velocity of a point rotating together with the body
Vec3 Body::getTangentialVel(Vec3 tpos)
{
	Vec3 lin_vel = this->ang_vel.cross(tpos - this->pos);
	return lin_vel;
}