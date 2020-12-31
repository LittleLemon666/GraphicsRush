#include "Object.H"
#include <time.h>

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
	
}

Obstacle::
Obstacle(float pos, int _lane, int _height, bool _black) : Object(pos, _lane, _height) {
	position = pos;
	lane = _lane;
	height = _height;
	black = _black;
}

void Obstacle::
addObstacleModel(std::string obj_path, std::string texture_path)
{
	obstacle_obj.push_back(new Model(obj_path));
	obstacle_texture.push_back(Texture2D(texture_path.c_str()));
}

void Obstacle::
loadObstacleModelInHpp()
{
	for (int _chapter = 0; _chapter < 4; _chapter++) {
		for (int error = 0; error < 4; error++) {
			obstacle_obj.push_back(new Model("CUBE"));
			obstacle_texture.push_back(Texture2D(obstacleTextures[_chapter][error].c_str()));
		}
	}
	//reversi blocks
	obstacle_obj.push_back(new Model("CUBE"));
	obstacle_texture.push_back(Texture2D("../GraphicsRush/Images/reversi/black.png"));
	obstacle_obj.push_back(new Model("CUBE"));
	obstacle_texture.push_back(Texture2D("../GraphicsRush/Images/reversi/white.png"));

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