#pragma once
#include <vector>

#include "Vector3.h"
#include "Body.h"

class SHGravity
{
public:
	int id;
	Body* parent;
	const char* modelFilename;
	int max_n;
	int max_m;

	double R_ref;
	// zonal and tesseral coefficients
	std::vector<std::vector<double>> C = std::vector<std::vector<double>>(1500, std::vector<double>(1500, 0));
	std::vector<std::vector<double>> S = std::vector<std::vector<double>>(1500, std::vector<double>(1500, 0));;

	SHGravity(int pid, Body* pparent, const char* pmodelFilename, int pmax_n, int pmax_m)
	{
		id = pid;
		parent = pparent;
		modelFilename = pmodelFilename;
		max_n = pmax_n;
		max_m = pmax_m;

		loadModel();
	}

	int loadModel();
	Vec3 computeAccel(Vec3 tpos, int n_max, int m_max);
};