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
	std::vector<Vessel*> vessels;
	Body* frame;
	Vec3 direction;
	std::string reldir;
	double delta_v;
	double perform_time;
	double prop_expenditure;

	// 0 = pending
	// 2 = completed
	int status = 0;

	ImpulsiveManeuver(int pid, std::vector<Vessel*> pvessels, Body* pframe, Vec3 pdir, std::string preldir,
		double pdelta_v, double pperform_time, double pprop_expenditure)
	{
		id = pid;
		vessels = pvessels;
		frame = pframe;
		direction = pdir;
		reldir = preldir;
		delta_v = pdelta_v;
		perform_time = pperform_time;
		prop_expenditure = pprop_expenditure;
	}

	void perform(double time);
};

class ConstAccelManeuver
{
public:
	int id;
	std::vector<Vessel*> vessels;
	Body* frame;
	Vec3 direction;
	std::string reldir;
	double delta_v;
	double accel;
	double perform_duration;
	double perform_time;
	double end_time;
	double prop_expenditure;

	// 0 = pending
	// 1 = in progress
	// 2 = completed
	int status = 0;

	ConstAccelManeuver(int pid, std::vector<Vessel*> pvessels, Body* pframe, Vec3 pdir,
		std::string preldir, double pdelta_v, double accel_duration,
		double pperform_time, std::string accel_or_duration, double pprop_expenditure)
	{
		id = pid;
		vessels = pvessels;
		frame = pframe;
		direction = pdir;
		reldir = preldir;
		delta_v = pdelta_v;

		// which one is specified, acceleration or the duration of the burn?
		if (accel_or_duration.compare("accel") == 0)
		{
			accel = accel_duration;
			perform_duration = delta_v / accel;
		}
		else // just assume the input is not sth else than either 'accel' or 'duration'
		{
			perform_duration = accel_duration;
			accel = delta_v / perform_duration;
		}

		perform_time = pperform_time;
		end_time = perform_time + perform_duration;

		prop_expenditure = pprop_expenditure;
	}

	void perform(double time);
};
