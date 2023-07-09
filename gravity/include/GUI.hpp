#pragma once

#include <SFML/Graphics.hpp>
#include "Asteroid.hpp"
#include <string>
#include <iostream>

class GUI
{
public:
	GUI();
	void update();
	void setWave(int wave_);
	int Score;
	int Lives;
	int Wave;
	std::string strLives;
	std::string strWave;
	sf::Font font;
	sf::Text score;
	sf::Text lives;
	sf::Text wave;
};
