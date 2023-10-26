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
	std::cout << "KOZMOWORKS Orbit Propagator\n\n";
	if (argc != 2) {
		std::cout << "Please enter a single mission filename to simulate. Quitting...\n\n";
		return 0;
	}

	char* mission_filename = argv[1];

	// read mission JSON
	std::vector<Body> bodies;
	std::vector<Vessel> vessels;
	std::vector<ImpulsiveManeuver> impulsive_maneuvers;

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

		ImpulsiveManeuver new_imp_mnv = ImpulsiveManeuver(new_id, new_vessel_ids, new_frame_id,
			new_direction, new_reldir, new_delta_v, new_perform_time);

		impulsive_maneuvers.push_back(new_imp_mnv);
	}

	// import simulation parameters
	std::cout << "Importing simulation parameters...\n";
	double time = mission_json["start_time"];
	double dt = mission_json["dt"];
	double end_time = mission_json["end_time"];

	// initialize solver
	std::cout << "Initializing solver...\n";
	Yoshida8 Y8 = Yoshida8(bodies, vessels, impulsive_maneuvers);

	// these two below are placeholders
	std::vector<Vec3> positions;
	std::vector<Vec3> positions2;

	// do physics
	std::cout << "Simulating...\n";
	while (time < end_time)
	{	
		Y8.step(dt, time);
		time += dt;

		positions.push_back(Y8.vessels[0].pos - Y8.bodies[0].pos);
		positions2.push_back(Y8.vessels[1].pos - Y8.bodies[0].pos);
	}

	// output positions to file
	// this should ideally be a modular thing
	// it stays here for now for testing purposes
	std::cout << "Writing results...\n";
	std::ofstream outfile;
	outfile.open("./output/output.csv");
	for (auto &pos : positions)
	{
		outfile << pos.x << ", " << pos.y << ", " << pos.z << "\n";
	}
	outfile.close();

	std::ofstream outfile2;
	outfile2.open("./output/output2.csv");
	for (auto& pos : positions2)
	{
		outfile2 << pos.x << ", " << pos.y << ", " << pos.z << "\n";
	}
	outfile2.close();

	std::cout << "Done!\n";
	return 0;
}