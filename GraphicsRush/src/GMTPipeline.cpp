#include "GMTPipeline.h"
void GMTPipeline::setG(int cp_id) {
	G = {
			{gmt_track->points[(cp_id - 1 + gmt_track->points.size()) % gmt_track->points.size()].pos.x, 
			gmt_track->points[cp_id % gmt_track->points.size()].pos.x, 
			gmt_track->points[(cp_id + 1) % gmt_track->points.size()].pos.x, 
			gmt_track->points[(cp_id + 2) % gmt_track->points.size()].pos.x},
			{ gmt_track->points[(cp_id - 1 + gmt_track->points.size()) % gmt_track->points.size()].pos.y, 
			gmt_track->points[cp_id % gmt_track->points.size()].pos.y, 
			gmt_track->points[(cp_id + 1) % gmt_track->points.size()].pos.y, 
			gmt_track->points[(cp_id + 2) % gmt_track->points.size()].pos.y },
			{ gmt_track->points[(cp_id - 1 + gmt_track->points.size()) % gmt_track->points.size()].pos.z, 
			gmt_track->points[cp_id % gmt_track->points.size()].pos.z, 
			gmt_track->points[(cp_id + 1) % gmt_track->points.size()].pos.z, 
			gmt_track->points[(cp_id + 2) % gmt_track->points.size()].pos.z },
			{ 1,1,1,1 }
	};
};
vec3 GMTPipeline::calculate(float t) {
	vec4 T = {t * t * t, t * t, t, 1.0f};
	vec4 result = transpose(G) * transpose(M) * T;
	return vec3(result);
};