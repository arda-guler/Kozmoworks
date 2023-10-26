#include <iostream>
#include "Maneuver.h"

void ImpulsiveManeuver::perform(double time, std::vector<Body>& bodies, std::vector<Vessel>& vessels)
{
	// check if it is not already performed and 
	// the sim time is the right time to perform, quit if not
	if (this->status != 0 || time < this->perform_time)
	{
		return;
	}

	// get the framebody
	Body frame = Body();
	for (auto b : bodies)
	{
		if (b.id == this->frame_id)
		{
			frame = b;
			break;
		}
	}

	// relative direction given, not vector
	// burn direction has to be calculated when the maneuver is to
	// be performed
	if (this->direction.mag() == 0)
	{
		// NOTE: if there are multiple vessels performing the same maneuver,
		// the first vessel in the vector is taken as reference
		Vessel primary_vessel = vessels[0];
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

	// everything is ready
	// perform the maneuver
	for (auto& v : vessels)
	{
		// check if vessel id is in maneuver vessel ids
		// (i.e. "Is this one of those vessels that are included in this maneuver?")
		for (auto test_id : this->vessel_ids) 
		{
			if (v.id == test_id)
			{
				v.vel = v.vel + this->direction * this->delta_v;
				break;
			}
		}
	}

	// the maneuver has been performed. make sure it is not performed again.
	this->status = 1;
}