#pragma once
#include <glad/glad.h>
#include "Model.H"
#include "RenderUtilities/Texture.h"
#include <vector>
#include <string>
#include "TrainWindow.H"
#include "Player.H"
using namespace glm;
const static std::vector<std::string> shop_items_models = { };
const static std::vector<std::string> shop_items_textures = { };
static enum { VER2, VER3, THIGH, SHADER, CUDA, REVERSI } item_name;

class Shop {
	public:
		Shop();
		void loadItemsModel();
		Model* shop_buttom = nullptr;
		Texture2D* shop_buttom_texture = nullptr;
		std::vector<Model*> items_obj;
		std::vector<Texture2D> items_texture;

		std::vector<int> item_price = { 100, 100, 100, 100, 100, 100 };
		void buy(TrainWindow* tw, int item_id);
};