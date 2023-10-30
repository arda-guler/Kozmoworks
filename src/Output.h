#include <string>

#include "Solver.h"

class Plot
{
public:

	Solver* s;
	int id;
	Vessel* target;
	Body* frame;
	std::string data_type;
	std::vector<std::vector<double>> output_data;

	Plot(Solver* ps, int pid, Vessel* ptarget, Body* pframe, std::string pdata_type)
	{
		id = pid;
		target = ptarget;
		frame = pframe;
		data_type = pdata_type;

		if (data_type.compare("pos") == 0)
		{
			std::vector<double> pos_x;
			std::vector<double> pos_y;
			std::vector<double> pos_z;
			output_data.push_back(pos_x);
			output_data.push_back(pos_y);
			output_data.push_back(pos_z);
		}
		else if (data_type.compare("vel") == 0)
		{
			std::vector<double> vel_x;
			std::vector<double> vel_y;
			std::vector<double> vel_z;
			output_data.push_back(vel_x);
			output_data.push_back(vel_y);
			output_data.push_back(vel_z);
		}
		else if (data_type.compare("accel") == 0)
		{
			std::vector<double> accel_x;
			std::vector<double> accel_y;
			std::vector<double> accel_z;
			output_data.push_back(accel_x);
			output_data.push_back(accel_y);
			output_data.push_back(accel_z);
		}
		else if (data_type.compare("mass") == 0)
		{
			std::vector<double> mass;
			output_data.push_back(mass);
		}
		else if (data_type.compare("prop_mass") == 0)
		{
			std::vector<double> prop_mass;
			output_data.push_back(prop_mass);
		}
	}

	void recordStep();
	void exportData();

};