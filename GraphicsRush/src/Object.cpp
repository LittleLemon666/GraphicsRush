#include "Object.H"

Model* Obstacle::obstacle_obj = nullptr;
Texture2D* Obstacle::obstacle_texture = nullptr;
std::string Obstacle::obstacle_obj_path = "../GraphicsRush/Objects/box.obj";
std::string Obstacle::obstacle_texture_path = "../GraphicsRush/Objects/box.jpg";

Object::Object(float pos, int _lane, int _height) {
	position = pos;
	lane = _lane;
	height = _height;
};

Obstacle::Obstacle(float pos, int _lane, int _height) : Object(pos, _lane, _height) {
	if (gladLoadGL())
	{
		if (!obstacle_obj)
			obstacle_obj = new Model(obstacle_obj_path);

		if (!obstacle_texture)
			obstacle_texture = new Texture2D(obstacle_texture_path.c_str());
	}
};

Money::Money(float pos, int _lane, int _height) : Object(pos, _lane, _height) {

};