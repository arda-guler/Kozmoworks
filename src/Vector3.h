#pragma once
#include <vector>

class Vec3
{
public:
	double x, y, z;

	Vec3() 
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vec3(double px, double py, double pz) 
	{
		x = px;
		y = py;
		z = pz;
	}

	Vec3(std::vector<double> pvec)
	{
		x = pvec[0];
		y = pvec[1];
		z = pvec[2];
	}

	Vec3 operator+ (Vec3 other)
	{
		double new_x = x + other.x;
		double new_y = y + other.y;
		double new_z = z + other.z;

		return Vec3(new_x, new_y, new_z);
	}

	Vec3 operator- (Vec3 other)
	{
		double new_x = x - other.x;
		double new_y = y - other.y;
		double new_z = z - other.z;

		return Vec3(new_x, new_y, new_z);
	}

	Vec3 operator* (double value)
	{
		double new_x = x * value;
		double new_y = y * value;
		double new_z = z * value;

		return Vec3(new_x, new_y, new_z);
	}

	Vec3 operator/ (double value)
	{
		double new_x = x / value;
		double new_y = y / value;
		double new_z = z / value;

		return Vec3(new_x, new_y, new_z);
	}

	Vec3 operator- ()
	{
		double new_x = -x;
		double new_y = -y;
		double new_z = -z;

		return Vec3(new_x, new_y, new_z);
	}

	Vec3 cross(Vec3 other);
	double dot(Vec3 other);
	Vec3 normalized();
	double mag();

};
