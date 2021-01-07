#pragma once
#include "Model.H"

class Rain
{
public:
	Rain() : num(2048), color(glm::vec3(0.0, 0.0, 1.0)), size(glm::vec3(100, 100, 100))
	{
		initRain();
	}
	Rain(int _num, glm::vec3 _color) : num(_num), color(_color), size(glm::vec3(100, 100, 100))
	{
		initRain();
	}
	void initRain()
	{
		rain_step = new int[num];
		position = new glm::vec3[num];
		rainball = new Model("LINE");
		for (int i = 0; i < num; i++)
		{
			rain_step[i] = rand() % (int)size.y;
			position[i] = glm::vec3(rand() % (int)size.x - size.x / 2, rand() % (int)size.y - size.y / 2, rand() % (int)size.z - size.z / 2);
		}
	}
	void rainBegin(glm::vec3 _position)
	{
		center = _position;
		rain_time = 0.0;
		raining = true;
	}
	void rainEnd()
	{
		raining = false;
	}
	void advanceRain()
	{
		rain_time++;
		if (rain_time >= size.y * 2) rain_time = 0;
	}
	const int getTime() const
	{
		return rain_time;
	}
	const int getRainStep(int index) const
	{
		return rain_step[index];
	}
	const float getRainNum() const
	{
		return num;
	}
	const bool isRaining() const
	{
		return raining;
	}
	const glm::vec3 getPos(int index) const
	{
		return center + position[index];
	}
	const void setPos(glm::vec3 pos)
	{
		center = pos;
	}
	void setColor(glm::vec3 _color)
	{
		color = _color;
	}
	const glm::vec3 getColor() const
	{
		return color;
	}
	const glm::vec3 getSize() const
	{
		return size;
	}
	void draw(int index)
	{
		rainball->draw();
	}
	~Rain()
	{
		delete rainball;
		delete[] position;
		delete[] rain_step;
	}

private:
	Model* rainball;
	int num;
	glm::vec3 color;
	glm::vec3 center;
	glm::vec3 size;
	glm::vec3* position;
	int rain_time = 0;
	int* rain_step;
	bool raining = false;

};