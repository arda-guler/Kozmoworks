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
#include "Atmosphere.h"
#include "RadiationPressure.h"

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

Body* findBodyViaID(int body_id, std::vector<Body>* bodies_ptr)
{
	for (auto& b : *bodies_ptr)
	{
		if (b.id == body_id)
		{
			return &b;
		}
	}

	std::cout << "Body not found!\n";
	return nullptr;
}

Vessel* findVesselViaID(int vessel_id, std::vector<Vessel>* vessels_ptr)
{
	for (auto& v : *vessels_ptr)
	{
		if (v.id == vessel_id)
		{
			return &v;
		}
	}

	std::cout << "Vessel not found!\n";
	return nullptr;
}

std::vector<Vessel*> findVesselsViaID(std::vector<int> vessel_ids, std::vector<Vessel>* vessels_ptr)
{
	std::vector<Vessel*> found_vessels_ptrs;
	for (auto search_id : vessel_ids)
	{
		for (auto& v : *vessels_ptr)
		{
			if (v.id == search_id)
			{
				found_vessels_ptrs.push_back(&v);
				break;
			}
		}
	}

	return found_vessels_ptrs;
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
	std::vector<PolyAtmo> poly_atmos;
	std::vector<ExpoAtmo> expo_atmos;
	std::vector<SphericalRadPress> sph_rad_presses;

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

		// get the framebody and vessels
		Body* new_frame_ptr = findBodyViaID(new_frame_id, &bodies);
		std::vector<Vessel*> new_vessels_ptr_vector = findVesselsViaID(new_vessel_ids, &vessels);

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

		// get the framebody and vessels
		Body* new_frame_ptr = findBodyViaID(new_frame_id, &bodies);
		std::vector<Vessel*> new_vessels_ptr_vector = findVesselsViaID(new_vessel_ids, &vessels);

		// initialize according to whether the acceleration or the duration is given
		if (new_accel == 0.0)
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

	// import atmospheric drag effects
	std::cout << "Importing atmospheric drag effects...\n";
	for (auto pad : mission_json["poly_atmos"])
	{
		int new_id = pad["id"];
		int new_target_id = pad["target_id"];
		int new_frame_id = pad["frame_id"];
		std::vector<double> new_poly_coeffs = pad["poly_coeffs"];
		double new_Cd = pad["Cd"];
		double new_area = pad["drag_area"];
		double new_max_alt = pad["max_alt"];

		Vessel* new_target_ptr = findVesselViaID(new_target_id, &vessels);
		Body* new_frame_ptr = findBodyViaID(new_frame_id, &bodies);

		double new_max_R = new_max_alt + new_frame_ptr->Rmin;

		PolyAtmo new_atmo_drag = PolyAtmo(new_id, new_poly_coeffs, new_Cd, new_area, new_max_R,
			new_frame_ptr, new_target_ptr);

		poly_atmos.push_back(new_atmo_drag);
	}

	for (auto ead : mission_json["expo_atmos"])
	{
		int new_id = ead["id"];
		int new_target_id = ead["target_id"];
		int new_frame_id = ead["frame_id"];
		double new_base_density = ead["base_density"];
		double new_scale_height = ead["scale_height"];
		double new_Cd = ead["Cd"];
		double new_area = ead["drag_area"];
		double new_max_alt = ead["max_alt"];

		Vessel* new_target_ptr = findVesselViaID(new_target_id, &vessels);
		Body* new_frame_ptr = findBodyViaID(new_frame_id, &bodies);

		double new_max_R = new_max_alt + new_frame_ptr->Rmin;

		ExpoAtmo new_expo_atmo = ExpoAtmo(new_id, new_base_density, new_scale_height, 
			new_Cd, new_area, new_max_R, new_frame_ptr, new_target_ptr);

		expo_atmos.push_back(new_expo_atmo);
	}

	// import radiation pressure effects
	std::cout << "Importing radiation pressure effects...\n";
	for (auto srp : mission_json["sph_rad_presses"])
	{
		int new_id = srp["id"];
		int new_target_id = srp["target_id"];
		int new_frame_id = srp["frame_id"];
		double new_luminosity = srp["luminosity"];
		double new_area = srp["rad_press_area"];
		double new_reflectivity = srp["reflectivity"];
		Vec3 new_normal = Vec3(srp["normal"]);

		Vessel* new_target_ptr = findVesselViaID(new_target_id, &vessels);
		Body* new_frame_ptr = findBodyViaID(new_frame_id, &bodies);

		SphericalRadPress new_sph_rad_press = SphericalRadPress(new_id, new_luminosity, new_area, 
			new_reflectivity, new_normal, new_frame_ptr, new_target_ptr);

		sph_rad_presses.push_back(new_sph_rad_press);
	}

	// import simulation parameters
	std::cout << "Importing simulation parameters...\n";
	double time = mission_json["start_time"];
	double dt = mission_json["dt"];
	double end_time = mission_json["end_time"];

	// initialize solver
	std::cout << "Initializing solver...\n";
	std::vector<Body>* bodies_ptr = &bodies;
	std::vector<Vessel>* vessels_ptr = &vessels;
	std::vector<ImpulsiveManeuver>* impulsive_maneuvers_ptr = &impulsive_maneuvers;
	std::vector<ConstAccelManeuver>* const_accel_maneuvers_ptr = &const_accel_maneuvers;
	std::vector<PolyAtmo>* poly_atmos_ptr = &poly_atmos;
	std::vector<ExpoAtmo>* expo_atmos_ptr = &expo_atmos;
	std::vector<SphericalRadPress>* sph_rad_presses_ptr = &sph_rad_presses;
	Yoshida8 Y8 = Yoshida8(bodies_ptr, vessels_ptr, impulsive_maneuvers_ptr, const_accel_maneuvers_ptr,
		poly_atmos_ptr, expo_atmos_ptr, sph_rad_presses_ptr);

	Yoshida8* Y8ptr = &Y8;

	// import plots
	std::cout << "Creating plots...\n";
	std::vector<Plot> plots;

	for (auto p : mission_json["plots"])
	{
		int new_id = p["id"];
		int new_target_id = p["target_id"];
		int new_frame_id = p["frame_id"];
		std::string new_data_type = p["data_type"];

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
		Vessel* new_target_ptr;
		for (auto& v : vessels)
		{
			if (v.id == new_target_id)
			{
				new_target_ptr = &v;
				break;
			}
		}

		Plot new_plot = Plot(Y8ptr, new_id, new_target_ptr, new_frame_ptr, new_data_type);
		plots.push_back(new_plot);
	}

	// do physics
	int cycles = 0;
	bool simulation_running = true;
	std::cout << "\n= = = SIMULATION STARTED = = =\n";
	while (time <= end_time && simulation_running)
	{	
		// update plots
		for (auto& p : plots)
		{
			p.recordStep();
		}

		// run physics solver
		Y8.step(dt, time);

		// update time
		cycles++;
		time = cycles * dt;

		// TEMPORARY: Check if any vessel has crashed
		for (auto &v: *Y8ptr->vessels)
		{
			for (auto& b : *Y8ptr->bodies)
			{
				if ((v.pos - b.pos).mag() < b.Rmax)
				{
					simulation_running = false;
				}
			}
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