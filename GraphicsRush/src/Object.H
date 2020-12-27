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
		static std::vector<Model*> obstacle_obj;
		static std::vector<Texture2D> obstacle_texture;
		static std::vector<Model*> money_obj;
		static std::vector<Texture2D> money_texture;
};

class Obstacle : public Object {
	public:
		Obstacle(float pos, int _lane, int _height);
		static void addObstacleModel(std::string obj_path, std::string texture_path);
		std::string type;
};

class Money : public Object {
	public:
		Money(float pos, int _lane, int _height);
		static void addMoneyModel(std::string obj_path, std::string texture_path);
};