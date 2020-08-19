#pragma once
#include "Snake.h"
int Snake::Body::count = 0;

void Snake::moving(int& button)
{
	m_prev_x = m_x;
	m_prev_y = m_y;
	switch (button)
	{
	case Keyboard::Left: m_x -= one_node; break;
	case Keyboard::Right: m_x += one_node; break;
	case Keyboard::Up: m_y -= one_node; break;
	case Keyboard::Down: m_y += one_node; break;
	}

	if (Body::count > 0)
	{
		body.at(0)->m_body_prev_x = body.at(0)->m_body_x;
		body.at(0)->m_body_prev_y = body.at(0)->m_body_y;
		body.at(0)->m_body_x = this->m_prev_x;
		body.at(0)->m_body_y = this->m_prev_y;
		if (Body::count > 1)
		{
			for (int j = 1; j < Body::count; j++)
			{
				body.at(j)->m_body_prev_x = body.at(j)->m_body_x;
				body.at(j)->m_body_prev_y = body.at(j)->m_body_y;
				body.at(j)->m_body_x = body.at(j - 1)->m_body_prev_x;
				body.at(j)->m_body_y = body.at(j - 1)->m_body_prev_y;
			}
		}
	}

	head.setPosition(m_x, m_y);
	if (Body::count > 0)
		for (auto& el : body)
		{
			el->shape_body.setPosition(el->m_body_x, el->m_body_y);
		}

}

void Snake::deleteBody()
{
	if (Body::count)
		for (auto el : body)
			delete el;
}

bool Snake::isCollision()//функция столкновения головы с объектом
{
	bool collision = false;
	if (this->m_x >= map_width || this->m_x < 0 || this->m_y >= map_height || this->m_y < 0)
		collision = true;

	if (Body::count)
		for (auto& el : body)
		{
			if (this->m_x == el->m_body_x && this->m_y == el->m_body_y)
				collision = true;
		}
	return collision;
}

void Snake::addBody()
{
	//if (Body::count == 0)// если тела еще нет, т.е. только одна голова, то создаем тело и присваем указатель на голову
		body.push_back(new Body(m_prev_x, m_prev_y));
	//else
	//{
	//	body.push_back(new Body(last->m_body_x, last->m_body_y));//указатель на предыдущий элемент
	//}
}
