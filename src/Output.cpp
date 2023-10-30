#include <iostream>
#include <fstream>

#include "Output.h"

void Plot::recordStep()
{
	if (this->frame->id > 0)
	{
		if (this->data_type.compare("pos") == 0)
		{
			this->output_data[0].push_back(this->target->pos.x);
			this->output_data[1].push_back(this->target->pos.y);
			this->output_data[2].push_back(this->target->pos.z);
		}
		else if (this->data_type.compare("vel") == 0)
		{
			this->output_data[0].push_back(this->target->vel.x);
			this->output_data[1].push_back(this->target->vel.y);
			this->output_data[2].push_back(this->target->vel.z);
		}
		else if (this->data_type.compare("accel") == 0)
		{
			this->output_data[0].push_back(this->target->accel.x);
			this->output_data[1].push_back(this->target->accel.y);
			this->output_data[2].push_back(this->target->accel.z);
		}
		else if (this->data_type.compare("mass") == 0)
		{
			this->output_data[0].push_back(this->target->mass);
		}
	}
	else 
	{
		if (this->data_type.compare("pos") == 0)
		{
			this->output_data[0].push_back(this->target->pos.x - this->frame->pos.x);
			this->output_data[1].push_back(this->target->pos.y - this->frame->pos.y);
			this->output_data[2].push_back(this->target->pos.z - this->frame->pos.z);
		}
		else if (this->data_type.compare("vel") == 0)
		{
			this->output_data[0].push_back(this->target->vel.x - this->frame->vel.x);
			this->output_data[1].push_back(this->target->vel.y - this->frame->vel.y);
			this->output_data[2].push_back(this->target->vel.z - this->frame->vel.z);
		}
		else if (this->data_type.compare("accel") == 0)
		{
			this->output_data[0].push_back(this->target->accel.x - this->frame->accel.x);
			this->output_data[1].push_back(this->target->accel.y - this->frame->accel.y);
			this->output_data[2].push_back(this->target->accel.z - this->frame->accel.z);
		}
		else if (this->data_type.compare("mass") == 0)
		{
			// I have no idea what this may be useful for at the moment, but it is 
			// easily implementable so it shall be implemented
			this->output_data[0].push_back(this->target->mass - this->frame->mass);
		}
	}
	
}

void Plot::exportData()
{
	std::ofstream outfile;
	std::string outfile_name = "./output/";
	outfile_name = outfile_name + std::to_string(this->id) + ".csv";
	std::cout << "Writing output file: " << outfile_name << "\n";
	outfile.open(outfile_name);
	
	for (int i = 0; i < this->output_data[0].size() - 1; i++)
	{
		for (auto data_column : this->output_data)
		{
			outfile << data_column[i] << ",";
		}
		outfile << "\n";
	}

	outfile.close();
}