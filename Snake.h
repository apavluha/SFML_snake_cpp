#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


const int rad = 10;
const int one_node = rad * 2;
const int map_height = 30 * one_node;
const int map_width = 30 * one_node;


class Snake
{
private:
	int m_prev_x;
	int m_prev_y;
	int m_x;
	int m_y;
	CircleShape head;

	

	class Body//Тело змейки
	{
	public:
		int m_body_prev_x;
		int m_body_prev_y;
		int m_body_x;
		int m_body_y;
		static int count;
		CircleShape shape_body;

		Body(int x, int y) :m_body_y(y), m_body_x(x)
		{
			count++;
			shape_body.setRadius(rad);
			shape_body.setFillColor(Color::Blue);
			shape_body.setPosition(m_body_x, m_body_y);
		}
	};

	std::vector<Body*> body;

public:
	Snake(int x = 10 * one_node, int y = 20 * one_node) :m_x(x), m_y(y)
	{
		m_prev_x = m_x-one_node;
		m_prev_y = m_y;
		head.setRadius(rad);
		head.setFillColor(Color::Red);
		head.setPosition(m_x, m_y);
		Body::count = 0;
	}

	void moving(int& button);
	int getCount() { return Body::count; }
	CircleShape& getHead() { return this->head; }
	CircleShape& getBodyShape(int index) { return this->body.at(index)->shape_body; }
	int getBodyX(int index) { return body.at(index)->m_body_x; }
	int getBodyY(int index) { return body.at(index)->m_body_y; }
	int getX() { return m_x; }
	int getY() { return m_y; }
	void addBody();
	bool isCollision();//функция столкновения головы с объектом
	void deleteBody();
	
	
	
};

