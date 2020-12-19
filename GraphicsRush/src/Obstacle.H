#pragma once
#include <glad/glad.h>
#include "Model.H"
#include "RenderUtilities/Texture.h"

class Obstacle {
	public:
		Obstacle(float pos, int _lane, int _height);
		int lane;
		int height;
		float position;
		static Model* obstacle_obj;
		static Texture2D* obstacle_texture;
		static std::string obstacle_obj_path;
		static std::string obstacle_texture_path;
};