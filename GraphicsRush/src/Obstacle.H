#pragma once
#include <glad/glad.h>
#include "Object.H"
#include "RenderUtilities/Texture.h"

class Obstacle {
	public:
		Obstacle(float pos, int _lane);
		int lane;
		float position;
		static Object* obstacle_obj;
		static Texture2D* obstacle_texture;
		static std::string obstacle_obj_path;
		static std::string obstacle_texture_path;
};