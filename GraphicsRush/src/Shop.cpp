#include "Shop.h"
Shop::Shop()
{
	if (gladLoadGL())
	{
		shop_buttom = new Model("../GraphicsRush/Objects/shop.obj");
		shop_buttom_texture = new Texture2D("../GraphicsRush/Objects/shop.jpg");
		loadItemsModel();
	}
}

void Shop::
loadItemsModel()
{
	for (int i = 0; i < shop_items_models.size(); i++)
	{
		items_obj.push_back(new Model(shop_items_models[i]));
		items_texture.push_back(Texture2D(shop_items_textures[i].c_str()));
	}
}