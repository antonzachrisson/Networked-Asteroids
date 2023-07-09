#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class UFO
{
public:
	sf::Vector2f position;
	sf::Vector2f velocity;
	int lives;
	int numFramesMove;
	sf::Texture Texture;
	void move();
	void changeDirection();
	UFO();
	void update(bool m_alive);
	int getLives();
	void removeLives();
	void reset();
	sf::Sprite Sprite;
	bool alive;
};

