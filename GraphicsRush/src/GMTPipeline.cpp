#include "GMTPipeline.h"
#include "Track.h"
void GMTPipeline::setG_pos(int cp_id) {
	G = {
			{gmt_track->points[(cp_id - 1 + (int)gmt_track->points.size()) % (int)gmt_track->points.size()].pos.x, 
			gmt_track->points[cp_id % (int)gmt_track->points.size()].pos.x, 
			gmt_track->points[(cp_id + 1) % (int)gmt_track->points.size()].pos.x, 
			gmt_track->points[(cp_id + 2) % (int)gmt_track->points.size()].pos.x},
			{ gmt_track->points[(cp_id - 1 + (int)gmt_track->points.size()) % (int)gmt_track->points.size()].pos.y, 
			gmt_track->points[cp_id % (int)gmt_track->points.size()].pos.y, 
			gmt_track->points[(cp_id + 1) % (int)gmt_track->points.size()].pos.y, 
			gmt_track->points[(cp_id + 2) % (int)gmt_track->points.size()].pos.y },
			{ gmt_track->points[(cp_id - 1 + (int)gmt_track->points.size()) % (int)gmt_track->points.size()].pos.z, 
			gmt_track->points[cp_id % (int)gmt_track->points.size()].pos.z, 
			gmt_track->points[(cp_id + 1) % (int)gmt_track->points.size()].pos.z, 
			gmt_track->points[(cp_id + 2) % (int)gmt_track->points.size()].pos.z },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
};
void GMTPipeline::setG_orient(int cp_id) {
	G = {
			{gmt_track->points[(cp_id - 1 + (int)gmt_track->points.size()) % (int)gmt_track->points.size()].orient.x,
			gmt_track->points[cp_id % (int)gmt_track->points.size()].orient.x,
			gmt_track->points[(cp_id + 1) % (int)gmt_track->points.size()].orient.x,
			gmt_track->points[(cp_id + 2) % (int)gmt_track->points.size()].orient.x},
			{ gmt_track->points[(cp_id - 1 + (int)gmt_track->points.size()) % (int)gmt_track->points.size()].orient.y,
			gmt_track->points[cp_id % (int)gmt_track->points.size()].orient.y,
			gmt_track->points[(cp_id + 1) % (int)gmt_track->points.size()].orient.y,
			gmt_track->points[(cp_id + 2) % (int)gmt_track->points.size()].orient.y },
			{ gmt_track->points[(cp_id - 1 + (int)gmt_track->points.size()) % (int)gmt_track->points.size()].orient.z,
			gmt_track->points[cp_id % (int)gmt_track->points.size()].orient.z,
			gmt_track->points[(cp_id + 1) % (int)gmt_track->points.size()].orient.z,
			gmt_track->points[(cp_id + 2) % (int)gmt_track->points.size()].orient.z },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
};
vec3 GMTPipeline::calculate(float t) {
	vec4 T = { t * t * t, t * t, t, 1.0f };
	vec4 result = transpose(G) * transpose(M) * T;
	return vec3(result);
};
void GMTPipeline::calculateAll(float trainU, vec3& position, vec3& forward, vec3& up, vec3& crossed) {
	setG_pos((int)trainU);
	float ratio = trainU - (int)trainU;
	position = calculate(ratio);
	//0.02 comes from 1 / PATH_DIVIDE when it's 50
	vec3 newPos = calculate(ratio + 0.0001f);
	forward = newPos - position;
	setG_orient((int)trainU);
	up = calculate(ratio);
	crossed = cross(forward, up);
	up = cross(crossed, forward);
};