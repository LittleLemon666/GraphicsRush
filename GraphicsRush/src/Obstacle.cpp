#include "Obstacle.H"

Object* Obstacle::obstacle_obj = nullptr;
Texture2D* Obstacle::obstacle_texture = nullptr;
std::string Obstacle::obstacle_obj_path = "../GraphicsRush/Objects/box.obj";
std::string Obstacle::obstacle_texture_path = "../GraphicsRush/Objects/box.jpg";
Obstacle::Obstacle(float pos, int _lane, int _height) {
	position = pos;
	lane = _lane;
	height = _height;
	if (gladLoadGL())
	{
		if (!obstacle_obj)
			obstacle_obj = new Object(obstacle_obj_path);

		if (!obstacle_texture)
			obstacle_texture = new Texture2D(obstacle_texture_path.c_str());
	}
};