#pragma once
#include <glad/glad.h>
#include "Model.H"
#include "RenderUtilities/Texture.h"
class Object {
	public:
		Object(float pos, int _lane, int _height);
		int lane;
		int height;
		float position;
};
class Obstacle : public Object {
	public:
		Obstacle(float pos, int _lane, int _height);
		static Model* obstacle_obj;
		static Texture2D* obstacle_texture;
		static std::string obstacle_obj_path;
		static std::string obstacle_texture_path;
};

class Money : public Object {
	public:
		Money(float pos, int _lane, int _height);
};