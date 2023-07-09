#pragma once

#include <SFML/Graphics.hpp>
#include "Asteroid.hpp"
#include <string>
#include <iostream>

class GUI
{
public:
	void getScore(Asteroids* asteroidsptr);
	void setScore();
	void setLives();
	int Score;
	int Lives;
	int Wave;
	std::string strLives;
	std::string strWave;
	GUI();
	void setWave(int wave_);
	void update(Asteroids* asteroidsptr);
	int getLives();
	void removeLives();
	sf::Font font;
	sf::Text score;
	sf::Text lives;
	sf::Text wave;
};
