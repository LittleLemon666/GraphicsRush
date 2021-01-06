#pragma once
#include "Model.H"

class Star
{
public:
	Star() : num(12), color(glm::vec3(1.0, 1.0, 0.0)), r(50.0), height(100.0)
	{
		initStar();
	}
	Star(int _num, glm::vec3 _color) : num(_num), color(_color), r(50.0), height(100.0)
	{
		initStar();
	}
	void initStar()
	{
		star_angle = new float[num];
		star = new Model("../GraphicsRush/Objects/star.obj");
		int first_angle = rand() % 360;
		for (int i = 0; i < num; i++)
		{
			star_angle[i] = (first_angle + i * 360 / num) % 360;
		}
	}
	void starBegin(glm::vec3 _position)
	{
		center = _position;
		star_time = 0.0;
		shiny = true;
	}
	void starEnd()
	{
		shiny = false;
	}
	void advanceStar()
	{
		star_time += 5;
		if (star_time >= 1080)
		{
			shiny = false;
			star_time = 0;
		}
	}
	const int getTime() const
	{
		return star_time;
	}
	const int getStarAngle(int index) const
	{
		return star_angle[index];
	}
	const float getStarNum() const
	{
		return num;
	}
	const bool isShiny() const
	{
		return shiny;
	}
	const glm::vec3 getPos() const
	{
		return center;
	}
	const void setPos(glm::vec3 pos)
	{
		center = pos;
	}
	const void setR(const float _r)
	{
		r = _r;
	}
	const float getR() const
	{
		return r;
	}
	const float getHeight() const
	{
		return height;
	}
	void setColor(glm::vec3 _color)
	{
		color = _color;
	}
	const glm::vec3 getColor() const
	{
		return color;
	}
	void draw(int index)
	{
		star->draw();
	}
	~Star()
	{
		delete star;
		delete[] star_angle;
	}

private:
	Model* star;
	int num;
	glm::vec3 color;
	glm::vec3 center;
	float r;
	float height;
	float* star_angle = 0;
	int star_time = 0;
	bool shiny = false;

};