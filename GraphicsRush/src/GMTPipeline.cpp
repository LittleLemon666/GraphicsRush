#include "GMTPipeline.h"
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
	vec4 T = {t * t * t, t * t, t, 1.0f};
	vec4 result = transpose(G) * transpose(M) * T;
	return vec3(result);
};