#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "SphericalHarmonics.h"
#include "Vector3.h"

int SHGravity::loadModel()
{
	std::ifstream mf(modelFilename);

	if (!mf.is_open())
	{
		std::cout << "ERROR: Could not open gravity model file " << modelFilename << "!\n";
		return 1;
	}

	std::vector<double> vn, vm;
	std::vector<std::vector<double>> vc(1500, std::vector<double>(1500, 0));
	std::vector<std::vector<double>> vs(1500, std::vector<double>(1500, 0));
	std::vector<double> firstLineData;
	std::string line;

	// read first line
	if (std::getline(mf, line)) 
	{
		std::istringstream ss(line);
		std::string token;

		while (std::getline(ss, token, ',')) 
		{
			try 
			{
				double value = std::stod(token);
				firstLineData.push_back(value);
			}
			catch (const std::invalid_argument& e) 
			{
				std::cout << "Invalid value in the first line: " << token << "\n";
				return 1;
			}
		}

		this->R_ref = firstLineData[0];
	}
	else 
	{
		std::cerr << "File is empty." << std::endl;
		return 1;
	}

	// read other lines with C and S values
	while (std::getline(mf, line))
	{
		std::istringstream ss(line);
		std::string token;
		std::vector<double> values;

		while (std::getline(ss, token, ','))
		{
			try
			{
				double value = std::stod(token);
				values.push_back(value);
			}
			catch (const std::invalid_argument& e)
			{
				std::cout << "Invalid value: " << token << "\n";
				return 1;
			}
		}

		if (values.size() == 4)
		{
			int n = (int)values[0];
			int m = (int)values[1];
			vc[n][m] = values[2];
			vs[n][m] = values[3];
		}
		else
		{
			std::cout << "Invalid number of values in line: " << line << "\n";
			return 1;
		}
	}

	this->C = vc;
	this->S = vs;

	mf.close();

	return 0;
}

/*
n_max: max. degree
m_max: max. order

https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20160011252.pdf
*/
void SHGravity::computeNormalization(int n_max)
{
	int m_max_temp = n_max;
	n_max += 1;

	for (int n = 2; n < n_max; n++)
	{
		norm1[n] = sqrt((2 * n + 1) / (2 * n - 1));
		norm2[n] = sqrt((2 * n + 1) / (2 * n - 3));
		norm11[n] = sqrt((2 * n + 1) / (2 * n)) / (2 * n - 1);
		normn10[n] = sqrt((n + 1) * n / 2);

		for (int m = 1; m < n; m++)
		{
			norm1m[n][m] = sqrt((n - m) * (2 * n + 1) / ((n + m) * (2 * n - 1)));
			norm2m[n][m] = sqrt((n - m) * (n - m - 1) * (2 * n + 1) / ((n + m) * (n + m - 1) * (2 * n - 3)));
			normn1[n][m] = sqrt((n + m + 1) * (n - m));
		}
	}
}

/*
n_max: max. degree
m_max: max. order

The original MATLAB code (from NASA) can be found at:
https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20160011252.pdf

Thank you NASA, very cool.

As is, this function does violent things of unspeakable nature to performance.
I am probably overlooking something obvious and fundamental here. Any improvements would be nice.
*/

Vec3 SHGravity::computeAccel(Vec3 tpos, int n_max, int m_max)
{
	// normalization factors computed
	Vec3 x = parent->getBodyCenteredCoords(tpos);
	double r = x.mag();
	double r_inv = 1 / r;

	double x_r = x.x * r_inv;
	double y_r = x.y * r_inv;
	double z_r = x.z * r_inv;

	double rp_r = R_ref * r_inv;
	double rp_rn = rp_r;

	double mu_r2 = parent->mu * r_inv * r_inv;

	std::vector<std::vector<double>> P(1500, std::vector<double>(1500, 0));
	P[0][0] = 1;
	P[1][0] = sqrt(3) * z_r;
	P[1][1] = sqrt(3);

	// sectorial ALFs
	for (int n = 2; n < n_max + 1; n++)
	{
		P[n][n] = norm11[n] * P[n - 1][n - 1] * (2 * n - 1);
	}

	std::vector<double> ctil(1500, 0);
	std::vector<double> stil(1500, 0);

	ctil[1] = x_r;
	stil[1] = y_r;

	double sumh = 0;
	double sumgm = 1;
	double sumj = 0;
	double sumk = 0;

	for (int n = 2; n < n_max + 1; n++)
	{
		rp_rn *= rp_r;

		int n2m1 = 2 * n - 1;
		int nm1 = n - 1;
		int nm2 = n - 2;
		int np1 = n + 1;

		// tesseral ALFs
		P[n][nm1] = normn1[n][nm1] * z_r * P[n][n];

		// zonal ALFs
		P[n][0] = (n2m1 * z_r * norm1[n] * P[nm1][0] - nm1 * norm2[n] * P[nm2][0]) / n;
		P[n][1] = (n2m1 * z_r * norm1m[n][1] * P[nm1][1] - n * norm2m[n][1] * P[nm2][1]) / nm1;

		double sumhn = normn10[n] * P[n][1] * C[n][0];
		double sumgmn = P[n][0] * C[n][0] * np1;

		if (m_max > 0)
		{
			for (int m = 2; m < n - 1; m++)
			{
				P[n][m] = (n2m1 * z_r * norm1m[n][m] * P[nm1][m] - 
					(nm1 + m) * norm2m[n][m] * P[nm2][m]) / (n - m);
			}

			double sumjn = 0;
			double sumkn = 0;

			ctil[n] = ctil[1] * ctil[nm1] - stil[1] * stil[nm1];
			stil[n] = stil[1] * ctil[nm1] + ctil[1] * stil[nm1];
			
			for (int m = 1; m < std::max(m_max, n) + 1; m++)
			{
				int mm1 = m - 1;
				int mp1 = m + 1;
				double mxpnm = m * P[n][m];

				double bnmtil = C[n][m] * ctil[m] + S[n][m] * stil[m];

				sumhn += normn1[n][m] * P[n][mp1] * bnmtil;
				sumgmn += (n + m + 1) * P[n][m] * bnmtil;

				double bnmtm1 = C[n][m] * ctil[mm1] + S[n][m] * stil[mm1];
				double anmtm1 = C[n][m] * stil[mm1] - S[n][m] * ctil[mm1];

				sumjn += mxpnm * bnmtm1;
				sumkn -= mxpnm * anmtm1;
			}

			sumj += rp_rn * sumjn;
			sumk += rp_rn * sumkn;
		}

		sumh += rp_rn * sumhn;
		sumgm += rp_rn * sumgmn;
	}

	double lambda = sumgm + z_r * sumh;

	double acc_x = -mu_r2 * (lambda * x_r - sumj);
	double acc_y = -mu_r2 * (lambda * y_r - sumk);
	double acc_z = -mu_r2 * (lambda * z_r - sumh);

	Vec3 acc = Vec3(acc_x, acc_y, acc_z);
	return acc;
}
