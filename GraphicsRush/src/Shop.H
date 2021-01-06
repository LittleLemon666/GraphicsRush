#pragma once
#include <glad/glad.h>
#include "Model.H"
#include "RenderUtilities/Texture.h"
#include <vector>
#include <string>
#include "TrainWindow.H"
#include "Player.H"
using namespace glm;
const static std::vector<std::string> shop_items_models = {
			"QUAD",
			"QUAD",
			"QUAD",
			"QUAD",
			"QUAD",
			"QUAD",
			"QUAD",
			"QUAD",
			"QUAD",
			"QUAD" };
const static std::vector<std::string> shop_items_textures = {
			"../GraphicsRush/Images/shop/ver2.png",
			"../GraphicsRush/Images/shop/ver3.png",
			"../GraphicsRush/Images/shop/chicken_leg.png",
			"../GraphicsRush/Images/shop/shader.png",
			"../GraphicsRush/Images/shop/cuda.png",
			"../GraphicsRush/Images/shop/checkpoint1.png",
			"../GraphicsRush/Images/shop/checkpoint2.png",
			"../GraphicsRush/Images/shop/checkpoint3.png" ,
			"../GraphicsRush/Images/shop/checkpoint4.png" ,
			"../GraphicsRush/Images/shop/checkpoint5.png" };
static enum { VER2, VER3, THIGH, SHADER, CUDA, CHECKPOINT1, CHECKPOINT2, CHECKPOINT3, CHECKPOINT4, CHECKPOINT5} item_name;
static enum { CP1, CP2, CP3, CP4, CP5 } cp_name;

class Shop {
	public:
		Shop();
		void loadItemsModel();
		Model* shop_buttom = nullptr;
		Texture2D* shop_buttom_texture = nullptr;
		std::vector<Model*> items_obj;
		std::vector<Texture2D> items_texture;
		std::vector<glm::vec3> items_pos;

		std::vector<int> item_price = { 100, 200, 100, 200, 200, 500, 100, 200, 300, 400 }; //github change this
		void buy(TrainWindow* tw, int item_id);
};