#pragma once
#include <glad/glad.h>
#include "Model.H"
#include "RenderUtilities/Texture.h"
#include <vector>
#include <string>
static int objectChapter = 0;
const static std::vector<std::vector<std::string>> obstacleTextures = {
	{ "../GraphicsRush/Objects/P1/dither-fs.jpg", "../GraphicsRush/Objects/P1/gauss.jpg", "../GraphicsRush/Objects/P1/pop.jpg", "../GraphicsRush/Objects/P1/rotate.jpg" }
, { "../GraphicsRush/Objects/P2/clip.jpg", "../GraphicsRush/Objects/P2/corner.jpg", "../GraphicsRush/Objects/P2/depth.jpg", "../GraphicsRush/Objects/P2/disappear.jpg" }
, { "../GraphicsRush/Objects/P3/arclength.jpg", "../GraphicsRush/Objects/P3/loop.jpg", "../GraphicsRush/Objects/P3/multiple.jpg", "../GraphicsRush/Objects/P3/trainview.jpg" }
, { "../GraphicsRush/Objects/P4/heightmap.jpg", "../GraphicsRush/Objects/P4/pixel.jpg", "../GraphicsRush/Objects/P4/reflect.jpg", "../GraphicsRush/Objects/P4/refract.jpg" } };
class Object {
	public:
		Object(float pos, int _lane, int _height);
		int lane;
		int height;
		float position;
		int type = rand() % 4;
		static std::vector<Model*> obstacle_obj;
		static std::vector<Texture2D> obstacle_texture;
		static std::vector<Model*> money_obj;
		static std::vector<Texture2D> money_texture;
};

class Obstacle : public Object {
	public:
		Obstacle(float pos, int _lane, int _height);
		static void addObstacleModel(std::string obj_path, std::string texture_path);
};

class Money : public Object {
	public:
		Money(float pos, int _lane, int _height);
		static void addMoneyModel(std::string obj_path, std::string texture_path);
};