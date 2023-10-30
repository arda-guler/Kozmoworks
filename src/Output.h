#include <string>

#include "Solver.h"

class Plot
{
public:

	Solver& s;
	int id;
	int target_id;
	int frame_id;
	std::string data_type;
	std::vector<std::vector<double>> output_data;

	Vessel* target;
	Body* frame;

	Plot(Solver &ps, int pid, int ptarget_id, int pframe_id, std::string pdata_type) : s(ps)
	{
		id = pid;
		target_id = ptarget_id;
		frame_id = pframe_id;
		data_type = pdata_type;

		// look for target
		for (auto& v : *s.vessels)
		{
			if (v.id == target_id)
			{
				target = &v;
				break;
			}
		}

		// look for frame
		if (frame_id > -1)
		{
			for (auto& b : *s.bodies)
			{
				if (b.id == frame_id)
				{
					frame = &b;
					break;
				}
			}
		}

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