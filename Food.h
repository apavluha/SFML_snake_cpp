#pragma once
#include "Snake.h"

class Food
{
private:
	int m_x;
	int m_y;
	Texture food_texture;
	Sprite food;


public:
	Food(Snake* snake);

	Sprite& getFood() { return this->food; }
	int getX() { return m_x; }
	int getY() { return m_y; }

};

