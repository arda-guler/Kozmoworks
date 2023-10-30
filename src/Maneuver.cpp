#include <iostream>
#include "Maneuver.h"

void ImpulsiveManeuver::perform(double time)
{
	// check if it is not already performed and 
	// the sim time is the right time to perform, quit if not
	if (this->status != 0 || time < this->perform_time)
	{
		return;
	}

	Body frame = *this->frame;
	
	// relative direction given, not vector
	// burn direction has to be calculated when the maneuver is to
	// be performed
	if (this->direction.mag() == 0)
	{
		// NOTE: if there are multiple vessels performing the same maneuver,
		// the first vessel in the vector is taken as reference
		Vessel primary_vessel = *this->vessels[0];
		if (this->reldir.compare("prograde") == 0)
		{
			this->direction = (primary_vessel.vel - frame.vel).normalized();
		}
		else if (this->reldir.compare("retrograde") == 0)
		{
			this->direction = -(primary_vessel.vel - frame.vel).normalized();
		}
		else if (this->reldir.compare("normal") == 0)
		{
			Vec3 dir_to_frame = (frame.pos - primary_vessel.pos).normalized();
			Vec3 vel_dir_to_frame = (primary_vessel.vel - frame.vel).normalized();
			this->direction = vel_dir_to_frame.cross(dir_to_frame);
		}
		else if (this->reldir.compare("antinormal") == 0)
		{
			Vec3 dir_to_frame = (frame.pos - primary_vessel.pos).normalized();
			Vec3 vel_dir_to_frame = (primary_vessel.vel - frame.vel).normalized();
			this->direction = -vel_dir_to_frame.cross(dir_to_frame);
		}
		else if (this->reldir.compare("radial_out") == 0)
		{
			this->direction = (primary_vessel.pos - frame.pos).normalized();
		}
		else if (this->reldir.compare("radial_in") == 0)
		{
			this->direction = (frame.pos - primary_vessel.pos).normalized();
		}
	}

	for (auto &v : this->vessels) 
	{

		v->vel = v->vel + this->direction * this->delta_v;
		v->mass = v->mass - this->prop_expenditure;
		v->prop_mass = v->prop_mass - this->prop_expenditure;
		if (v->prop_mass < 0 || v->mass < 0)
		{
			std::cout << "\nWARNING: A vessel was left with a negative wet mass or negative propellant mass after an orbital maneuver!\n";
		}
		break;
	}

	// The maneuver has been performed. Make sure it is not performed again.
	this->status = 2;
}

void ConstAccelManeuver::perform(double time)
{
	// check if it is not already performed and 
	// the sim time is the right time to perform, quit if not
	if (this->status == 2 || time < this->perform_time)
	{
		return;
	}

	// this maneuver has ended
	if (this->status != 2 && time > this->end_time)
	{
		for (auto& v : this->vessels)
		{
			v->mass = v->mass - this->prop_expenditure;
			v->prop_mass = v->prop_mass - this->prop_expenditure;

			if (v->prop_mass < 0 || v->mass < 0)
			{
				std::cout << "\nWARNING: A vessel was left with a negative wet mass or negative propellant mass after an orbital maneuver!\n";
			}
		}
		this->status = 2;
		return;
	}

	// this maneuver has just begun, mark it so
	if (this->status == 0)
	{
		this->status = 1;
	}

	Body frame = *this->frame;
	
	// relative direction given, not vector
	// burn direction has to be calculated when the maneuver is to
	// be performed
	bool clear_direction_flag = false;
	if (this->direction.mag() == 0)
	{
		clear_direction_flag = true;
		// NOTE: if there are multiple vessels performing the same maneuver,
		// the first vessel in the vector is taken as reference
		Vessel primary_vessel = *this->vessels[0];
		if (this->reldir.compare("prograde") == 0)
		{
			this->direction = (primary_vessel.vel - frame.vel).normalized();
		}
		else if (this->reldir.compare("retrograde") == 0)
		{
			this->direction = -(primary_vessel.vel - frame.vel).normalized();
		}
		else if (this->reldir.compare("normal") == 0)
		{
			Vec3 dir_to_frame = (frame.pos - primary_vessel.pos).normalized();
			Vec3 vel_dir_to_frame = (primary_vessel.vel - frame.vel).normalized();
			this->direction = vel_dir_to_frame.cross(dir_to_frame);
		}
		else if (this->reldir.compare("antinormal") == 0)
		{
			Vec3 dir_to_frame = (frame.pos - primary_vessel.pos).normalized();
			Vec3 vel_dir_to_frame = (primary_vessel.vel - frame.vel).normalized();
			this->direction = -vel_dir_to_frame.cross(dir_to_frame);
		}
		else if (this->reldir.compare("radial_out") == 0)
		{
			this->direction = (primary_vessel.pos - frame.pos).normalized();
		}
		else if (this->reldir.compare("radial_in") == 0)
		{
			this->direction = (frame.pos - primary_vessel.pos).normalized();
		}
	}

	// if a relative direction was given rather than a vector value,
	// it should be updated each frame - clear the vector
	if (clear_direction_flag) 
	{
		this->direction = Vec3();
	}
}
