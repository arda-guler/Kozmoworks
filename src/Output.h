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

	int data_type_tag;

	Plot(Solver* ps, int pid, Vessel* ptarget, Body* pframe, std::string pdata_type)
	{
		id = pid;
		target = ptarget;
		frame = pframe;
		data_type = pdata_type;

		/* 
		this if-else if ladder assigns a data_type_tag to the options so we can
		easily use a switch-case instead of an if-else ladder at simulation runtime
		*/
		if (data_type.compare("pos") == 0)
		{
			std::vector<double> pos_x;
			std::vector<double> pos_y;
			std::vector<double> pos_z;
			output_data.push_back(pos_x);
			output_data.push_back(pos_y);
			output_data.push_back(pos_z);

			data_type_tag = 1;
		}
		else if (data_type.compare("vel") == 0)
		{
			std::vector<double> vel_x;
			std::vector<double> vel_y;
			std::vector<double> vel_z;
			output_data.push_back(vel_x);
			output_data.push_back(vel_y);
			output_data.push_back(vel_z);

			data_type_tag = 2;
		}
		else if (data_type.compare("accel") == 0)
		{
			std::vector<double> accel_x;
			std::vector<double> accel_y;
			std::vector<double> accel_z;
			output_data.push_back(accel_x);
			output_data.push_back(accel_y);
			output_data.push_back(accel_z);

			data_type_tag = 3;
		}
		else if (data_type.compare("mass") == 0)
		{
			std::vector<double> mass;
			output_data.push_back(mass);

			data_type_tag = 4;
		}
		else if (data_type.compare("prop_mass") == 0)
		{
			std::vector<double> prop_mass;
			output_data.push_back(prop_mass);

			data_type_tag = 5;
		}
		else if (data_type.compare("R") == 0 || data_type.compare("pos_mag") == 0)
		{
			std::vector<double> R;
			output_data.push_back(R);

			data_type_tag = 6;
		}
		else if (data_type.compare("alt") == 0)
		{
			std::vector<double> alt;
			output_data.push_back(alt);

			data_type_tag = 7;
		}
		else if (data_type.compare("vel_mag") == 0)
		{
			std::vector<double> vel_mag;
			output_data.push_back(vel_mag);

			data_type_tag = 8;
		}
		else if (data_type.compare("accel_mag") == 0)
		{
			std::vector<double> accel_mag;
			output_data.push_back(accel_mag);

			data_type_tag = 9;
		}
	}

	void recordStep();
	void exportData();

};