#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Vector3.h"
#include "Matrix3x3.h"
#include "RigidBody.h"
#include "Body.h"
#include "Vessel.h"
#include "Solver.h"
#include "Maneuver.h"
#include "Output.h"

#include "include/nlohmann/json.hpp"
using json = nlohmann::json;

json readJSON(std::string filename)
{
	std::ifstream file;
	file.open(filename);
	json data = json::parse(file);
	file.close();
	return data;
}

int main(int argc, char **argv)
{
	std::cout << "\nKOZMOWORKS Astrodynamics Simulator\n\n";
	if (argc != 2) {
		std::cout << "Please enter a single mission filename to simulate. Quitting...\n\n";
		return 0;
	}

	char* mission_filename = argv[1];

	// read mission JSON
	std::vector<Body> bodies;
	std::vector<Vessel> vessels;
	std::vector<ImpulsiveManeuver> impulsive_maneuvers;
	std::vector<ConstAccelManeuver> const_accel_maneuvers;

	std::cout << "Reading mission file:" << mission_filename << "\n";
	json mission_json = readJSON(mission_filename);
	
	// import bodies
	std::cout << "Importing bodies...\n";
	for (auto b : mission_json["bodies"])
	{
		int new_id = b["id"];
		Vec3 new_pos = Vec3(b["pos"]);
		Vec3 new_vel = Vec3(b["vel"]);
		Vec3 new_accel = Vec3();
		Mtx3x3 new_orient = Mtx3x3(b["orient"]);
		Vec3 new_ang_vel = Vec3(b["ang_vel"]);
		Vec3 new_ang_accel = Vec3();
		double new_mass = b["mass"];
		Mtx3x3 new_MoI = Mtx3x3(b["moment_of_inertia"]);
		double new_Rmin = b["Rmin"];
		double new_Rmax = b["Rmax"];
		Body new_body = Body(new_id, new_pos, new_vel, new_accel,
			new_orient, new_ang_vel, new_ang_accel, new_mass,
			new_MoI, new_Rmin, new_Rmax);

		bodies.push_back(new_body);
	}

	// import vessels
	std::cout << "Importing vessels...\n";
	for (auto v : mission_json["vessels"])
	{
		int new_id = v["id"];
		Vec3 new_pos = Vec3(v["pos"]);
		Vec3 new_vel = Vec3(v["vel"]);
		Vec3 new_accel = Vec3();
		Mtx3x3 new_orient = Mtx3x3(v["orient"]);
		Vec3 new_ang_vel = Vec3(v["ang_vel"]);
		Vec3 new_ang_accel = Vec3();
		double new_mass = v["mass"];
		Mtx3x3 new_MoI = Mtx3x3(v["moment_of_inertia"]);
		double new_prop_mass = v["prop_mass"];
		Vessel new_vessel = Vessel(new_id, new_pos, new_vel, new_accel,
			new_orient, new_ang_vel, new_ang_accel, new_mass,
			new_MoI, new_prop_mass);

		vessels.push_back(new_vessel);
	}

	// import impulsive maneuvers
	std::cout << "Importing impulsive maneuvers...\n";
	for (auto im : mission_json["impulsive_maneuvers"])
	{
		int new_id = im["id"];
		std::vector<int> new_vessel_ids = im["vessel_ids"];
		int new_frame_id = im["frame_id"];
		Vec3 new_direction = Vec3(im["direction"]);
		std::string new_reldir = im["rel_dir"];
		double new_delta_v = im["delta_v"];
		double new_perform_time = im["perform_time"];
		double new_prop_expenditure = im["prop_expenditure"];

		// get the framebody
		Body* new_frame_ptr;
		for (auto &b : bodies)
		{
			if (b.id == new_frame_id)
			{
				new_frame_ptr = &b;
				break;
			}
		}

		// get the vessels
		std::vector<Vessel*> new_vessels_ptr_vector;
		for (auto& v : vessels)
		{
			// check if vessel id is in maneuver vessel ids
			// (i.e. "Is this one of those vessels that are included in this maneuver?")
			for (auto test_id : new_vessel_ids)
			{
				if (v.id == test_id)
				{
					new_vessels_ptr_vector.push_back(&v);
					break;
				}
			}
		}

		ImpulsiveManeuver new_imp_mnv = ImpulsiveManeuver(new_id, new_vessels_ptr_vector, new_frame_ptr,
			new_direction, new_reldir, new_delta_v, new_perform_time, new_prop_expenditure);

		impulsive_maneuvers.push_back(new_imp_mnv);
	}

	// import constant acceleration maneuvers
	std::cout << "Importing constant acceleration maneuvers...\n";
	for (auto cam : mission_json["const_accel_maneuvers"])
	{
		int new_id = cam["id"];
		std::vector<int> new_vessel_ids = cam["vessel_ids"];
		int new_frame_id = cam["frame_id"];
		Vec3 new_direction = Vec3(cam["direction"]);
		std::string new_reldir = cam["rel_dir"];
		double new_delta_v = cam["delta_v"];
		double new_accel = cam["accel"];
		double new_duration = cam["duration"];
		double new_perform_time = cam["perform_time"];
		double new_prop_expenditure = cam["prop_expenditure"];

		// get the framebody
		Body* new_frame_ptr;
		for (auto& b : bodies)
		{
			if (b.id == new_frame_id)
			{
				new_frame_ptr = &b;
				break;
			}
		}

		// get the vessels
		std::vector<Vessel*> new_vessels_ptr_vector;
		for (auto& v : vessels)
		{
			// check if vessel id is in maneuver vessel ids
			// (i.e. "Is this one of those vessels that are included in this maneuver?")
			for (auto test_id : new_vessel_ids)
			{
				if (v.id == test_id)
				{
					new_vessels_ptr_vector.push_back(&v);
					break;
				}
			}
		}

		if (new_accel == 0)
		{
			ConstAccelManeuver new_const_accel_mnv = ConstAccelManeuver(new_id, new_vessels_ptr_vector, new_frame_ptr,
				new_direction, new_reldir, new_delta_v, new_duration, new_perform_time, "duration", new_prop_expenditure);

			const_accel_maneuvers.push_back(new_const_accel_mnv);
		}
		else
		{
			ConstAccelManeuver new_const_accel_mnv = ConstAccelManeuver(new_id, new_vessels_ptr_vector, new_frame_ptr,
				new_direction, new_reldir, new_delta_v, new_accel, new_perform_time, "accel", new_prop_expenditure);

			const_accel_maneuvers.push_back(new_const_accel_mnv);
		}
	}

	// import simulation parameters
	std::cout << "Importing simulation parameters...\n";
	double time = mission_json["start_time"];
	double dt = mission_json["dt"];
	double end_time = mission_json["end_time"];

	// initialize solver
	std::cout << "Initializing solver...\n";
	Yoshida8 Y8 = Yoshida8(&bodies, &vessels, &impulsive_maneuvers, &const_accel_maneuvers);

	// import plots
	std::cout << "Creating plots...\n";
	std::vector<Plot> plots;

	for (auto p : mission_json["plots"])
	{
		int new_id = p["id"];
		int new_target_id = p["target_id"];
		int new_frame_id = p["frame_id"];
		std::string new_data_type = p["data_type"];

		Plot new_plot = Plot(Y8, new_id, new_target_id, new_frame_id, new_data_type);
		plots.push_back(new_plot);
	}

	// do physics
	std::cout << "\n= = = SIMULATION STARTED = = =\n";
	while (time < end_time)
	{	
		Y8.step(dt, time);
		time += dt;

		for (auto& p : plots)
		{
			p.recordStep();
		}
	}
	std::cout << "= = =  SIMULATION ENDED  = = =\n\n";

	// output recorded data to files
	std::cout << "Writing results...\n";
	for (auto& p : plots)
	{
		p.exportData();
	}

	std::cout << "Done!\n";
	return 0;
}