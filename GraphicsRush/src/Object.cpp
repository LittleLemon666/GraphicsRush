#include "Object.H"

std::vector<Model*> Object::obstacle_obj;
std::vector<Texture2D> Object::obstacle_texture;
std::vector<Model*> Object::money_obj;
std::vector<Texture2D> Object::money_texture;

Object::Object(float pos, int _lane, int _height) {
	position = pos;
	lane = _lane;
	height = _height;
};

Obstacle::
Obstacle(float pos, int _lane, int _height) : Object(pos, _lane, _height) {
	type = "default";
}

void Obstacle::
addObstacleModel(std::string obj_path, std::string texture_path)
{
	if (gladLoadGL())
	{
		obstacle_obj.push_back(new Model(obj_path));
		obstacle_texture.push_back(Texture2D(texture_path.c_str()));
	}
}

Money::Money(float pos, int _lane, int _height) : Object(pos, _lane, _height) {

}

void Money::
addMoneyModel(std::string obj_path, std::string texture_path)
{
	if (gladLoadGL())
	{
		money_obj.push_back(new Model(obj_path));
		money_texture.push_back(Texture2D(texture_path.c_str()));
	}
}