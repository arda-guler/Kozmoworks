#pragma once
#include <vector>

#include "RigidBody.h"
#include "Body.h"
#include "Vessel.h"
#include "Maneuver.h"

class Solver
{
public:
	std::vector<Body> bodies;
	std::vector<Vessel> vessels;
	std::vector<ImpulsiveManeuver> impulsive_maneuvers;

	Solver() {}

};

class SymplecticEuler : public Solver
{
public:

	SymplecticEuler(std::vector<Body> pbodies, std::vector<Vessel> pvessels,
		std::vector<ImpulsiveManeuver> pimpulsive_maneuvers)
	{
		bodies = pbodies;
		vessels = pvessels;
		impulsive_maneuvers = pimpulsive_maneuvers;
	}

	void step(double dt, double time);
};

class Yoshida8 : public Solver
{
public:

	Yoshida8(std::vector<Body> pbodies, std::vector<Vessel> pvessels,
		std::vector<ImpulsiveManeuver> pimpulsive_maneuvers)
	{
		bodies = pbodies;
		vessels = pvessels;
		impulsive_maneuvers = pimpulsive_maneuvers;
	}

	void step(double dt, double time);
};