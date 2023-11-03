#pragma once
#include "Vessel.h"
#include "Body.h"
#include "Vector3.h"

class SphericalRadPress
{
public:
	int id;
	double luminosity;
	double area;
	double reflectivity;
	Vec3 normal;
	Body* parent;
	Vessel* target;

	SphericalRadPress(int pid, double pluminosity, double parea, double preflectivity,
		Vec3 pnormal, Body* pparent, Vessel* pvessel)
	{
		id = pid;
		luminosity = pluminosity;
		area = parea;
		reflectivity = preflectivity;
		normal = pnormal;
		parent = pparent;
		target = pvessel;
	}

	// computes and applies the acceleration to the target vessel
	void apply();
};