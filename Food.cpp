#pragma once
#include "Food.h"

Food::Food(Snake* snake)
{
	bool isEqual;
	do {
		isEqual = false;
		m_x = rand() % (map_width - 10);
		m_y = rand() % (map_height - 10);

		if (m_x % one_node >= rad)
			while (m_x % one_node != 0)
				m_x++;
		if (m_x % one_node < rad && m_x % one_node != 0)
			while (m_x % one_node != 0)
				m_x--;

		if (m_y % one_node >= rad)
			while (m_y % one_node != 0)
				m_y++;
		if (m_y % one_node < rad && m_y % one_node != 0)
			while (m_y % one_node != 0)
				m_y--;

		if (snake->getCount() > 0)
			for (int i = 1; i <= snake->getCount(); i++)
			{
				if (snake->getBodyX(i - 1) == m_x && snake->getBodyY(i - 1) == m_y)
				{
					isEqual = true;
				}
			}


	} while (isEqual);

	food_texture.loadFromFile("apple.png");
	food.setTexture(food_texture);
	food.setPosition(m_x, m_y);
}