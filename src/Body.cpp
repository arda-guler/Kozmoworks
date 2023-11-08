#include "Body.h"
#include "constants.h"

Vec3 Body::getGravity(Vec3 tpos, bool harmonics=false)
{
	// point mass gravity
	if (!harmonics) 
	{
		Vec3 grav_dir = (this->pos - tpos).normalized();
		double dist = (tpos - this->pos).mag();
		double grav_mag = this->mu / (dist * dist);
		return grav_dir * grav_mag;
	}
	else
	{
		// the gravitational acceleration caused by this body
		// will be handled by a spherical harmonics computation
		return Vec3();
	}
}

// get the linear velocity of a point rotating together with the body
Vec3 Body::getTangentialVel(Vec3 tpos)
{
	Vec3 lin_vel = this->ang_vel.cross(tpos - this->pos);
	return lin_vel;
}

Vec3 Body::getBodyCenteredCoords(Vec3 abspos)
{
	Vec3 relpos = abspos - this->pos;
	double delta_x = relpos.x;
	double delta_y = relpos.y;
	double delta_z = relpos.z;

	Vec3 converted = Vec3((delta_x * this->orient.m11) + (delta_y * this->orient.m12) + (delta_z * this->orient.m13),
						  (delta_x * this->orient.m21) + (delta_y * this->orient.m22) + (delta_z * this->orient.m23),
						  (delta_x * this->orient.m31) + (delta_y * this->orient.m32) + (delta_z * this->orient.m33));

	return converted;
}