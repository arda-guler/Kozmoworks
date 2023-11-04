#include <iostream>
#include <fstream>

#include "Output.h"

void Plot::recordStep()
{
	switch (this->data_type_tag)
	{
	case 1: // pos
		this->output_data[0].push_back(this->target->pos.x - this->frame->pos.x);
		this->output_data[1].push_back(this->target->pos.y - this->frame->pos.y);
		this->output_data[2].push_back(this->target->pos.z - this->frame->pos.z);
		break;
	case 2: // vel
		this->output_data[0].push_back(this->target->vel.x - this->frame->vel.x);
		this->output_data[1].push_back(this->target->vel.y - this->frame->vel.y);
		this->output_data[2].push_back(this->target->vel.z - this->frame->vel.z);
		break;
	case 3: // accel
		this->output_data[0].push_back(this->target->accel.x - this->frame->accel.x);
		this->output_data[1].push_back(this->target->accel.y - this->frame->accel.y);
		this->output_data[2].push_back(this->target->accel.z - this->frame->accel.z);
		break;
	case 4: // mass
		this->output_data[0].push_back(this->target->mass);
		break;
	case 5: // propellant mass
		this->output_data[0].push_back(this->target->prop_mass);
		break;
	case 6: // R / pos mag
		this->output_data[0].push_back((this->target->pos - this->frame->pos).mag());
		break;
	case 7: // alt
		// TODO: Implement an R-at-latitude func and use that instead of an Rmin comparison
		this->output_data[0].push_back((this->target->pos - this->frame->pos).mag() - this->frame->Rmin);
		break;
	case 8: // vel mag
		this->output_data[0].push_back((this->target->vel - this->frame->vel).mag());
	case 9: // accel mag
		this->output_data[0].push_back((this->target->accel - this->frame->accel).mag());
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

void TimePlot::recordStep(double t)
{
	switch (this->data_type_tag) 
	{
	case 1: // seconds
		this->times.push_back(this->start_time + t);
		break;
	case 2: // days
		this->times.push_back(this->start_time + t / 86400);
		break;
	case 3: // years
		this->times.push_back(this->start_time + t / 31536000);
	}
}

void TimePlot::exportData()
{
	std::ofstream outfile;
	std::string outfile_name = "./output/";
	outfile_name = outfile_name + std::to_string(this->id) + ".csv";
	std::cout << "Writing output file: " << outfile_name << "\n";
	outfile.open(outfile_name);

	for (auto timepoint : this->times)
	{
		outfile << timepoint << "\n";
	}

	outfile.close();
}