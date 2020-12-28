#pragma once
#include <glad/glad.h>
#include "Model.H"
#include "RenderUtilities/Texture.h"
#include <vector>
#include <string>
using namespace glm;
const static std::vector<std::string> shop_items_models = { };
const static std::vector<std::string> shop_items_textures = { };
class Shop {
	public:
		Shop();
		void loadItemsModel();
		Model* shop_buttom = nullptr;
		Texture2D* shop_buttom_texture = nullptr;
		std::vector<Model*> items_obj;
		std::vector<Texture2D> items_texture;
};