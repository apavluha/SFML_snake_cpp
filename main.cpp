#include "Food.h"
#include <ctime>
#include <exception>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace sf;
static int button = Keyboard::Right;
bool in_game = false;
int select_menu = 0;
int enter = 0;
unsigned player_score = 0;
bool is_name_entered = false;
std::string str_name = "";

class PlayerScore
{
private:
	Text m_text;
	std::pair<int, int> m_start_line;

public:
	PlayerScore(std::string &str, Font &font):m_start_line(10,10)
	{
		m_text.setFont(font);
		m_text.setString(str);
		m_text.setCharacterSize(20);
		m_text.setFillColor(Color::Red);
		m_text.setPosition(m_start_line.first, m_start_line.second);
	}
	void updatePos()
	{
		m_text.setPosition(m_start_line.first, m_start_line.second);
	}

	void setPosY(int y)
	{
		m_text.setPosition(m_start_line.first, m_start_line.second + y);
	}
	Text& getText() { return this->m_text; }
	const std::string& getString() { return this->m_text.getString(); }
};

void inputPlayer(int& button, Event &event)
{
	if (event.key.code == Keyboard::Left && button != Keyboard::Right && in_game)
		button = Keyboard::Left;
	if (event.key.code == Keyboard::Right && button != Keyboard::Left && in_game)
		button = Keyboard::Right;
	if (event.key.code == Keyboard::Up && button != Keyboard::Down && in_game)
		button = Keyboard::Up;
	if (event.key.code == Keyboard::Down && button != Keyboard::Up && in_game)
		button = Keyboard::Down;
}

void getEvent(RenderWindow &window)
{
	Event event;
	
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
		
		inputPlayer(button, event);

		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == Keyboard::Down && !in_game)
				++select_menu;
			if (event.key.code == Keyboard::Up && !in_game)
				++select_menu;
			if (event.key.code == Keyboard::Enter && !in_game)
				enter = 1;
		}
	}
}


void getEventEnterName(RenderWindow& window, Text& text)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();

		if (event.type == Event::TextEntered && is_name_entered)
		{
			if (event.text.unicode < 128)
			{
				str_name += static_cast<char>(event.text.unicode);
				text.setString(str_name);
			}
		}

		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == Keyboard::Enter && !in_game)
				enter = 1;
		}
	}
}

void isNameEntered(RenderWindow& window, RectangleShape& map_field)
{
	Font font;
	font.loadFromFile("qqq.ttf");

	Text text_for_name("Enter your name: ", font, 20);
	text_for_name.setFillColor(Color::Red);

	Text text_name("", font, 20);
	text_name.setFillColor(Color::Red);
	text_name.setPosition(170, 0);

	enter = 0;

	while (window.isOpen())
	{
		getEventEnterName(window, text_name);
		if (enter) 
		{
			enter = 0;
			break;
		}
		window.clear(Color::White);
		window.draw(map_field);
		window.draw(text_for_name);
		window.draw(text_name);
		window.display();
	}
}

void isGame(RenderWindow &window, RectangleShape &map_field)
{
	Snake* snake = new Snake();//выдел€ем пам€ть под голову змеи и создаем ее на карте
	Food* food = new Food(snake);// выдел€ем пам€ть под еду и создаем ее на карте
	snake->addBody();
	snake->addBody();

	double timer = 0, delay = 0.1; //задерка перемещени€
	Clock clock;

	Font font;
	font.loadFromFile("qqq.ttf");

	Text game_over("GAME OVER", font, 40);
	game_over.setPosition(210, 250);
	game_over.setFillColor(Color::Red);

	player_score = 0;
	in_game = true;
	button = Keyboard::Right;

	while (window.isOpen())
	{
		double time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		getEvent(window);//обработчик нажати€ клавиш
		if (in_game && !snake->isCollision())
		{
			if (food->getX() == snake->getX() && food->getY() == snake->getY())
			{
				delete food;
				food = new Food(snake);
				snake->addBody();
				++player_score;
			}
			if (timer > delay)
			{
				snake->moving(button);
				timer = 0;
			}
			window.clear(Color::White);
			window.draw(map_field);
			window.draw(snake->getHead());
			window.draw(food->getFood());
			for (int i = 0; i < snake->getCount(); i++)
			{
				window.draw(snake->getBodyShape(i));
			}
		}
		else
		{	
			if (in_game)
			{
				snake->deleteBody();
				delete snake;
				delete food;
			}
			in_game = false;
			window.clear(Color::White);
			window.draw(map_field);
			window.draw(game_over);

			if (timer > 3)
			{
				is_name_entered = true;
				isNameEntered(window, map_field);
				break;
			}
		}
		window.display();

	}
}

void showScore(RenderWindow &window, RectangleShape &map_field, std::vector<PlayerScore> &arr)
{
	enter = 0;
	Font font;
	font.loadFromFile("qqq.ttf");
	std::string s = "No points yet!";
	PlayerScore fileEmpty(s, font);
	
	while (window.isOpen())
	{
		int next_line = 0;
		getEvent(window);


		window.clear(Color::White);
		window.draw(map_field);

		if (!arr.empty())
		for (auto it = arr.begin(); it != arr.end(); ++it)
		{
			it->setPosY(next_line);
			window.draw(it->getText());
			next_line += 20;
		}
		else
		{
			window.draw(fileEmpty.getText());
		}
		window.display();

		if (enter == 1)
			break;
	}
}



int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(map_width, map_height), "pidor"); //окно игры
	
	RectangleShape map_field(Vector2f(map_width, map_height)); // игровое поле
	map_field.setFillColor(Color::Yellow); // цвет игрового пол€

	Font font;
	font.loadFromFile("qqq.ttf");

	Text menu_play("PLAY", font, 40);
	menu_play.setPosition(240, 230);
	menu_play.setFillColor(Color::Red);

	Text menu_score("SCORE", font, 40);
	menu_score.setPosition(240, 270);
	menu_score.setFillColor(Color::Red);

	std::vector<PlayerScore> str_arr_score;

	std::string myFile = "SnakeScores.txt";
	std::fstream scores; // объ€вл€ем экземпл€р класса файлового ввода и вывода

	while (window.isOpen())
	{
		enter = 0;
		getEvent(window);
		if (select_menu %2 == 0)
		{
			menu_play.setStyle(Text::Underlined);
			menu_score.setStyle(Text::Underlined & 0);
		}
		if (select_menu % 2 == 1)
		{
			menu_play.setStyle(Text::Underlined & 0);
			menu_score.setStyle(Text::Underlined);
		}

		if (enter == 1 && select_menu % 2 == 0)
		{
			enter = 0;
			isGame(window, map_field);
			scores.open(myFile, std::ios::app); // открываем файл дл€ записи
			if (!scores.is_open())
			{
				window.close();
			}
			else
			{
				scores.seekg(std::ios::end); //перемещаем указатель в конец файла
				scores <<  str_name << " " << player_score << std::endl; // записываем им€ и очки в файл
				str_name = "";
			}
			scores.close(); // закрываем файл
		}
		if (enter == 1 && select_menu % 2 == 1)
		{
			enter = 0;
			scores.open(myFile);
			if (!scores.is_open())//если файл не открылс€, то выведетс€ сообщение об отсутствие рейтинга
			{
				showScore(window, map_field, str_arr_score);//процедура вывода рейтинга игроков
				enter = 0;
			}
			else
			{
				enter = 0;
				str_arr_score.clear();
				while (scores)
				{
					std::string str;
					std::getline(scores, str);//получаем данные рейтинга в строку и выводим результат
					str_arr_score.push_back(PlayerScore(str, font));//передаем строку в конструктор и объект помещаем в конец контейнера
				}
			}
			scores.close();
			showScore(window, map_field, str_arr_score);//процедура вывода рейтинга игроков
		}
		window.clear(Color::White);
		window.draw(map_field);
		window.draw(menu_play);
		window.draw(menu_score);
		window.display();
	}
	return 0;
}