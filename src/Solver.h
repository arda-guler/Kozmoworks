#pragma once
#include <vector>

#include "RigidBody.h"
#include "Body.h"
#include "Vessel.h"
#include "Maneuver.h"
#include "Atmosphere.h"
#include "RadiationPressure.h"
#include "SphericalHarmonics.h"

class Solver
{
public:
	std::vector<Body>* bodies;
	std::vector<Vessel>* vessels;
	std::vector<ImpulsiveManeuver>* impulsive_maneuvers;
	std::vector<ConstAccelManeuver>* const_accel_maneuvers;
	std::vector<PolyAtmo>* poly_atmos;
	std::vector<ExpoAtmo>* expo_atmos;
	std::vector<SphericalRadPress>* sph_rad_presses;
	std::vector<SHGravity>* shgravities;

	Solver() {}

};

// 1st order symplectic method
class SymplecticEuler : public Solver
{
public:

	SymplecticEuler(std::vector<Body>* pbodies, std::vector<Vessel>* pvessels,
		std::vector<ImpulsiveManeuver>* pimpulsive_maneuvers,
		std::vector<ConstAccelManeuver>* pconst_accel_maneuvers,
		std::vector<PolyAtmo>* ppoly_atmos, std::vector<ExpoAtmo>* pexpo_atmos,
		std::vector<SphericalRadPress>* psph_rad_presses,
		std::vector<SHGravity>* pshgravities)
	{
		bodies = pbodies;
		vessels = pvessels;
		impulsive_maneuvers = pimpulsive_maneuvers;
		const_accel_maneuvers = pconst_accel_maneuvers;
		poly_atmos = ppoly_atmos;
		expo_atmos = pexpo_atmos;
		sph_rad_presses = psph_rad_presses;
		shgravities = pshgravities;
	}

	void step(double dt, double time);
};

// 8th order symplectic method
class Yoshida8 : public Solver
{
public:

	Yoshida8(std::vector<Body>* pbodies, std::vector<Vessel>* pvessels,
		std::vector<ImpulsiveManeuver>* pimpulsive_maneuvers,
		std::vector<ConstAccelManeuver>* pconst_accel_maneuvers,
		std::vector<PolyAtmo>* ppoly_atmos, std::vector<ExpoAtmo>* pexpo_atmos,
		std::vector<SphericalRadPress>* psph_rad_presses,
		std::vector<SHGravity>* pshgravities)
	{
		bodies = pbodies;
		vessels = pvessels;
		impulsive_maneuvers = pimpulsive_maneuvers;
		const_accel_maneuvers = pconst_accel_maneuvers;
		poly_atmos = ppoly_atmos;
		expo_atmos = pexpo_atmos;
		sph_rad_presses = psph_rad_presses;
		shgravities = pshgravities;
	}

	void step(double dt, double time);
};