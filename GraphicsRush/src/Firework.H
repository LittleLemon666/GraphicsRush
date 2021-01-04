#pragma once
#include "Model.H"

class Firework
{
public:
	Firework() : num(128), color(glm::vec3(1.0))
	{
		initFirework();
	}
	Firework(int _num, glm::vec3 _color) : num(_num), color(_color)
	{
		initFirework();
	}
	void initFirework()
	{
		fireball = new Model * [num];
		dir = new glm::vec3[num];
		for (int i = 0; i < num; i++)
		{
			fireball[i] = new Model("SPHERE");
			dir[i] = glm::normalize(glm::vec3(rand() % 256 / 256.0f - 0.5f, rand() % 256 / 256.0f - 0.5f, rand() % 256 / 256.0f - 0.5f));
		}
	}
	void fireworkBegin(glm::vec3 _center)
	{
		center = _center;
		firework_time = 0.0;
		shoot = true;
	}
	void advanceFirework()
	{
		firework_time += 0.2f;
		if (firework_time < time_offset) center = glm::vec3(0.0, firework_time / time_offset * height, 0.0);
		else if (firework_time < 1.5 * time_offset) center = glm::vec3(0.0, height, 0.0);
		else shoot = false;
	}
	const float getTime() const
	{
		return firework_time;
	}
	const float getTimeOffset() const
	{
		return time_offset;
	}
	const float getScale() const
	{
		return (firework_time < time_offset ? 1.0f : firework_time - time_offset);
	}
	const float getFireballNum() const
	{
		return num;
	}
	const bool isShoot() const
	{
		return shoot;
	}
	const glm::vec3 getPos(int index) const
	{
		if (firework_time < time_offset) return position + center;
		return position + center + dir[index] * (firework_time - time_offset) * len;
	}
	const void setPos(glm::vec3 pos)
	{
		position = pos;
	}
	const glm::vec3 getColor() const
	{
		return color;
	}
	void draw(int index)
	{
		fireball[index]->draw();
	}
	~Firework()
	{
		delete[] fireball;
		delete[] dir;
	}

private:
	Model** fireball;
	int num;
	glm::vec3 color;
	glm::vec3 center;
	glm::vec3 position;
	glm::vec3* dir = nullptr;
	float firework_time = 0.0f;
	float time_offset = 10.0f;
	float scaling = 1.0f;
	float height = 100.0f;
	float len = 20.0f;
	bool shoot = false;

};