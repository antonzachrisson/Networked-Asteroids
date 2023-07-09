#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <cmath>

class Asteroids
{
public:
	sf::Vector2f position;
	char scale;
	sf::Texture Texture;
	Asteroids(char scale_, sf::Vector2f position_);
	Asteroids();
	~Asteroids();
	std::vector<Asteroids*> asteroids;
	sf::Sprite Sprite;
	int numAsteroids;
};
