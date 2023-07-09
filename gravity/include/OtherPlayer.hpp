#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class OtherPlayer
{
public:
	OtherPlayer(sf::Vector2f pos);
	sf::Vector2f position;
	float rotation;
	sf::Texture Texture;
	sf::Sprite Sprite;
	void Update();
};
