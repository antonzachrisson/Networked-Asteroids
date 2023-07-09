#include "OtherPlayer.hpp"
#include <iostream>

OtherPlayer::OtherPlayer(sf::Vector2f pos)
{
	position = pos;
	rotation = 90.0f;
	if (!Texture.loadFromFile("assets/Player.png"))
	{
		std::cout << "error loading file" << std::endl;
		return;
	}
	Sprite.setTexture(Texture);
	Sprite.setPosition(pos);
	Sprite.setOrigin(9.5f, 10.0f);
	Sprite.setRotation(90.0f);
};


void OtherPlayer::Update()
{
	Sprite.setPosition(position);
	Sprite.setRotation(rotation);
}