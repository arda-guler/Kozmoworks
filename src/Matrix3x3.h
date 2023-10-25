#pragma once
#include <vector>

#include "Vector3.h"

class Mtx3x3
{
public:
	double m11, m12, m13, m21, m22, m23, m31, m32, m33;

	// default to identity matrix
	Mtx3x3() {
		m11 = 1;
		m12 = 0;
		m13 = 0;

		m21 = 0;
		m22 = 1;
		m23 = 0;

		m31 = 0;
		m32 = 0;
		m33 = 1;
	}

	Mtx3x3(double p11, double p12, double p13, double p21, double p22, double p23, double p31, double p32, double p33) {
		m11 = p11;
		m12 = p12;
		m13 = p13;

		m21 = p21;
		m22 = p22;
		m23 = p23;

		m31 = p31;
		m32 = p32;
		m33 = p33;
	}

	Mtx3x3(std::vector<double> v1, std::vector<double> v2, std::vector<double> v3)
	{
		m11 = v1[0];
		m12 = v1[1];
		m13 = v1[2];

		m21 = v2[0];
		m22 = v2[1];
		m23 = v2[2];

		m31 = v3[0];
		m32 = v3[1];
		m33 = v3[2];
	}

	Mtx3x3(std::vector<std::vector<double>> v)
	{
		m11 = v[0][0];
		m12 = v[0][1];
		m13 = v[0][2];

		m21 = v[1][0];
		m22 = v[1][1];
		m23 = v[1][2];

		m31 = v[2][0];
		m32 = v[2][1];
		m33 = v[2][2];
	}

	Mtx3x3(Vec3 v1, Vec3 v2, Vec3 v3)
	{
		m11 = v1.x;
		m12 = v1.y;
		m13 = v1.z;

		m21 = v2.x;
		m22 = v2.y;
		m23 = v2.z;

		m31 = v3.x;
		m32 = v3.y;
		m33 = v3.z;
	}

	Mtx3x3 operator* (double value)
	{
		double new_m11 = this->m11 * value;
		double new_m12 = this->m12 * value;
		double new_m13 = this->m13 * value;

		double new_m21 = this->m21 * value;
		double new_m22 = this->m22 * value;
		double new_m23 = this->m23 * value;

		double new_m31 = this->m31 * value;
		double new_m32 = this->m32 * value;
		double new_m33 = this->m33 * value;

		return Mtx3x3(new_m11, new_m12, new_m13, new_m21, new_m22, new_m23, new_m31, new_m32, new_m33);
	}

	Mtx3x3 operator/ (double value)
	{
		double new_m11 = this->m11 / value;
		double new_m12 = this->m12 / value;
		double new_m13 = this->m13 / value;

		double new_m21 = this->m21 / value;
		double new_m22 = this->m22 / value;
		double new_m23 = this->m23 / value;

		double new_m31 = this->m31 / value;
		double new_m32 = this->m32 / value;
		double new_m33 = this->m33 / value;

		return Mtx3x3(new_m11, new_m12, new_m13, new_m21, new_m22, new_m23, new_m31, new_m32, new_m33);
	}

	Mtx3x3 operator* (Mtx3x3 other)
	{
		double new_m11 = this->m11 * other.m11 + this->m12 * other.m21 + this->m13 * other.m31;
		double new_m12 = this->m11 * other.m12 + this->m12 * other.m22 + this->m13 * other.m32;
		double new_m13 = this->m11 * other.m13 + this->m12 * other.m23 + this->m13 * other.m33;

		double new_m21 = this->m21 * other.m11 + this->m22 * other.m21 + this->m23 * other.m31;
		double new_m22 = this->m21 * other.m12 + this->m22 * other.m22 + this->m23 * other.m32;
		double new_m23 = this->m21 * other.m13 + this->m22 * other.m23 + this->m23 * other.m33;

		double new_m31 = this->m31 * other.m11 + this->m32 * other.m21 + this->m33 * other.m31;
		double new_m32 = this->m31 * other.m12 + this->m32 * other.m22 + this->m33 * other.m32;
		double new_m33 = this->m31 * other.m13 + this->m32 * other.m23 + this->m33 * other.m33;

		return Mtx3x3(new_m11, new_m12, new_m13, new_m21, new_m22, new_m23, new_m31, new_m32, new_m33);
	}

	Mtx3x3 operator+ (Mtx3x3 other)
	{
		double new_m11 = this->m11 + other.m11;
		double new_m12 = this->m12 + other.m12;
		double new_m13 = this->m13 + other.m13;

		double new_m21 = this->m21 + other.m21;
		double new_m22 = this->m22 + other.m22;
		double new_m23 = this->m23 + other.m23;

		double new_m31 = this->m31 + other.m31;
		double new_m32 = this->m32 + other.m32;
		double new_m33 = this->m33 + other.m33;

		return Mtx3x3(new_m11, new_m12, new_m13, new_m21, new_m22, new_m23, new_m31, new_m32, new_m33);
	}

	Mtx3x3 operator- (Mtx3x3 other)
	{
		double new_m11 = this->m11 - other.m11;
		double new_m12 = this->m12 - other.m12;
		double new_m13 = this->m13 - other.m13;

		double new_m21 = this->m21 - other.m21;
		double new_m22 = this->m22 - other.m22;
		double new_m23 = this->m23 - other.m23;

		double new_m31 = this->m31 - other.m31;
		double new_m32 = this->m32 - other.m32;
		double new_m33 = this->m33 - other.m33;

		return Mtx3x3(new_m11, new_m12, new_m13, new_m21, new_m22, new_m23, new_m31, new_m32, new_m33);
	}

	Mtx3x3 elementwiseMul(Mtx3x3 other)
	{
		double new_m11 = this->m11 * other.m11;
		double new_m12 = this->m12 * other.m12;
		double new_m13 = this->m13 * other.m13;

		double new_m21 = this->m21 * other.m21;
		double new_m22 = this->m22 * other.m22;
		double new_m23 = this->m23 * other.m23;

		double new_m31 = this->m31 * other.m31;
		double new_m32 = this->m32 * other.m32;
		double new_m33 = this->m33 * other.m33;

		return Mtx3x3(new_m11, new_m12, new_m13, new_m21, new_m22, new_m23, new_m31, new_m32, new_m33);
	}

	Mtx3x3 elementwiseDiv(Mtx3x3 other)
	{
		double new_m11 = this->m11 / other.m11;
		double new_m12 = this->m12 / other.m12;
		double new_m13 = this->m13 / other.m13;

		double new_m21 = this->m21 / other.m21;
		double new_m22 = this->m22 / other.m22;
		double new_m23 = this->m23 / other.m23;

		double new_m31 = this->m31 / other.m31;
		double new_m32 = this->m32 / other.m32;
		double new_m33 = this->m33 / other.m33;

		return Mtx3x3(new_m11, new_m12, new_m13, new_m21, new_m22, new_m23, new_m31, new_m32, new_m33);
	}

	Mtx3x3 rotated(double angle, Vec3 v)
	{
		Mtx3x3 m;
		double c = cos(angle);
		double s = sin(angle);
		m.m11 = v.x * v.x * (1 - c) + c;
		m.m12 = v.x * v.y * (1 - c) - v.z * s;
		m.m13 = v.x * v.z * (1 - c) + v.y * s;
		m.m21 = v.y * v.x * (1 - c) + v.z * s;
		m.m22 = v.y * v.y * (1 - c) + c;
		m.m23 = v.y * v.z * (1 - c) - v.x * s;
		m.m31 = v.x * v.z * (1 - c) - v.y * s;
		m.m32 = v.y * v.z * (1 - c) + v.x * s;
		m.m33 = v.z * v.z * (1 - c) + c;
		return m;
	}

	Mtx3x3 inverse()
	{
		Mtx3x3 m;

		// find the determinant
		double det = this->m11 * (this->m22 * this->m33 - this->m32 * this->m23) -
			this->m12 * (this->m21 * this->m33 - this->m23 * this->m31) +
			this->m13 * (this->m21 * this->m32 - this->m22 * this->m31);

		double inv_det = 1 / det;

		m.m11 = (this->m22 * this->m33 - this->m32 * this->m23) * inv_det;
		m.m12 = (this->m13 * this->m32 - this->m12 * this->m33) * inv_det;
		m.m13 = (this->m12 * this->m23 - this->m13 * this->m22) * inv_det;
		m.m21 = (this->m23 * this->m31 - this->m21 * this->m33) * inv_det;
		m.m22 = (this->m11 * this->m33 - this->m13 * this->m31) * inv_det;
		m.m23 = (this->m21 * this->m13 - this->m11 * this->m23) * inv_det;
		m.m31 = (this->m21 * this->m32 - this->m31 * this->m22) * inv_det;
		m.m32 = (this->m31 * this->m12 - this->m11 * this->m32) * inv_det;
		m.m33 = (this->m11 * this->m22 - this->m21 * this->m12) * inv_det;

		return m;
	}

	Vec3 dot(Vec3 v)
	{
		Vec3 res;

		res.x = this->m11 * v.x + this->m12 * v.y + this->m13 * v.z;
		res.y = this->m21 * v.x + this->m22 * v.y + this->m23 * v.z;
		res.z = this->m31 * v.x + this->m32 * v.y + this->m33 * v.z;

		return res;
	}
};

