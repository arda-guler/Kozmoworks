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
	std::vector<std::vector<double>> S = std::vector<std::vector<double>>(1500, std::vector<double>(1500, 0));

	std::vector<double> norm1 = std::vector<double>(1500, 0);
	std::vector<double> norm2 = std::vector<double>(1500, 0);
	std::vector<double> norm11 = std::vector<double>(1500, 0);
	std::vector<double> normn10 = std::vector<double>(1500, 0);

	std::vector<std::vector<double>> norm1m = std::vector<std::vector<double>>(1500, std::vector<double>(1500, 0));
	std::vector<std::vector<double>> norm2m = std::vector<std::vector<double>>(1500, std::vector<double>(1500, 0));
	std::vector<std::vector<double>> normn1 = std::vector<std::vector<double>>(1500, std::vector<double>(1500, 0));

	SHGravity(int pid, Body* pparent, const char* pmodelFilename, int pmax_n, int pmax_m)
	{
		id = pid;
		parent = pparent;
		modelFilename = pmodelFilename;
		max_n = pmax_n;
		max_m = pmax_m;

		loadModel();
		computeNormalization(max_n);
	}

	int loadModel();
	void computeNormalization(int n_max);
	Vec3 computeAccel(Vec3 tpos, int n_max, int m_max);
};