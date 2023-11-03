#include "RadiationPressure.h"
#include "constants.h"

void SphericalRadPress::apply()
{
	Vec3 R = this->target->pos - this->parent->pos;
	double R_mag = R.mag();
	double power_per_area = this->luminosity / (R_mag * R_mag);

	Vec3 ray_dir = R.normalized();
	Vec3 neg_normal = -this->normal;

	double cos_angle = ray_dir.dot(neg_normal);
	double power_received = power_per_area * area * cos_angle;

	// N = kg m s-2 = W / (m s-1)
	Vec3 normal_dir_force = this->normal * 2 * power_received * cos_angle / Constants::c;
	Vec3 ray_dir_force = ray_dir * power_received / Constants::c;

	Vec3 resultant_force = normal_dir_force * this->reflectivity + ray_dir_force * (1 - this->reflectivity);

	Vec3 resultant_accel = resultant_force / this->target->mass;

	this->target->applyAccel(resultant_accel);
}
