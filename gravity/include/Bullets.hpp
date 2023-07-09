#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <cmath>

class Bullets
{
public:
	Bullets(sf::Vector2f position_, float rot, bool player);
	~Bullets();
	sf::Vector2f position;
	sf::RectangleShape shape{ sf::Vector2f(3.0f, 10.0f) };
	std::vector<Bullets*> bullets;
	std::vector<Bullets*> ufobullets;
	int numbullets = 0;
	int ufonumbullets = 0;
};
