#pragma once
#include <vector>
#include <string>

#include "RigidBody.h"
#include "Vessel.h"
#include "Body.h"
#include "Vector3.h"

class ImpulsiveManeuver
{
public:
	int id;
	std::vector<int> vessel_ids;
	int frame_id;
	int status = 0;
	Vec3 direction;
	std::string reldir;
	double delta_v;
	double perform_time;

	// you have to init the vessels like this
	ImpulsiveManeuver(int pid, std::vector<int> pvessel_ids, int pframe_id,
		Vec3 pdirection, std::string preldir, double pdelta_v, double pperform_time)
	{
		id = pid;
		vessel_ids = pvessel_ids;
		frame_id = pframe_id;
		direction = pdirection;
		reldir = preldir;
		delta_v = pdelta_v;
		perform_time = pperform_time;
	}

	void perform(double time, std::vector<Body>&, std::vector<Vessel>&);
};
