#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class UFO
{
public:
	UFO();
	sf::Vector2f position;
	sf::Texture Texture;
	sf::Sprite Sprite;
	bool alive;
	void Update();
};

