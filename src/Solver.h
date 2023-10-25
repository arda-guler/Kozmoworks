#pragma once
#include <vector>

#include "RigidBody.h"
#include "Body.h"
#include "Vessel.h"

class Solver
{
public:
	std::vector<Body> bodies;
	std::vector<Vessel> vessels;

	Solver() {}

};

class SymplecticEuler : public Solver
{
public:

	SymplecticEuler(std::vector<Body> pbodies, std::vector<Vessel> pvessels)
	{
		bodies = pbodies;
		vessels = pvessels;
	}

	void step(double dt);
};

class Yoshida8 : public Solver
{
public:

	Yoshida8(std::vector<Body> pbodies, std::vector<Vessel> pvessels)
	{
		bodies = pbodies;
		vessels = pvessels;
	}

	void step(double dt);
};