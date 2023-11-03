#pragma once
#include "Body.h"
#include "Vessel.h"
#include "Vector3.h"

// a simplistic atmosphere with its density defined as a polynomial fit
class PolyAtmo
{
public:
	int id;
	std::vector<double> coeffs;
	double Cd;
	double area;
	double max_R;
	Body* parent;
	Vessel* target;

	double drag_multiplier; // = 0.5 * Cd * area, some const. value

	// make sure the polynomial coefficients are in descending order
	// i.e. x^n, x^(n-1), ... x^1, x^0

	// see https://www.spaceacademy.net.au/watch/debris/atmosmod.htm for an
	// Earth atmosphere approximation (watch the units!)

	// linear velocity = angular velocity CROSS position from center
	PolyAtmo(int pid, std::vector<double> pcoeffs, double pCd,
		double parea, double pmax_R, Body* pparent, Vessel* ptarget)
	{
		id = pid;
		coeffs = pcoeffs;
		Cd = pCd;
		area = parea;
		max_R = pmax_R;
		parent = pparent;
		target = ptarget;

		drag_multiplier = 0.5 * Cd * area;
	}

	// computes and applies the acceleration to the vessel
	void apply();
};

// another simplistic atmosphere with its density defined as rho = base * exp(-alt / scale_height)
class ExpoAtmo
{
public:
	int id;
	double base_density;
	double scale_height;
	double Cd;
	double area;
	double max_R;
	Body* parent;
	Vessel* target;

	double drag_multiplier; // = 0.5 * Cd * area, some const. value

	ExpoAtmo(int pid, double pbase_density, double pscale_height, double pCd,
		double parea, double pmax_R, Body* pparent, Vessel* ptarget)
	{
		id = pid;
		base_density = pbase_density;
		scale_height = pscale_height;
		Cd = pCd;
		area = parea;
		max_R = pmax_R;
		parent = pparent;
		target = ptarget;

		drag_multiplier = 0.5 * Cd * area;
	}

	// computes and applies the acceleration to the vessel
	void apply();
};