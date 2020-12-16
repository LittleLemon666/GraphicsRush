#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Track.h"
using namespace glm;
class GMTPipeline {
	public:
		CTrack* gmt_track;

		mat4 G = {};
		//M is defaulted to BSpline
		mat4 M = {
			{-1.0f / 6.0f, 3.0f / 6.0f, -3.0f / 6.0f, 1.0f / 6.0f},
			{3.0f / 6.0f, -6.0f / 6.0f, 0.0f / 6.0f, 4.0f / 6.0f},
			{-3.0f / 6.0f, 3.0f / 6.0f, 3.0f / 6.0f, 1.0f / 6.0f},
			{1.0f / 6.0f, 0.0f / 6.0f, 0.0f / 6.0f, 0.0f / 6.0f} };
		void setG(int cp_id);

		vec3 calculate(float t);
};