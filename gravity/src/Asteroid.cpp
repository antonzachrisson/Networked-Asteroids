#include "Asteroid.hpp"

Asteroids::Asteroids(char scale_, sf::Vector2f position_)
{
	numAsteroids = 0;
	position = position_;
	scale = scale_;
	
	if (!Texture.loadFromFile("assets/Asteroid.png"))
	{
		std::cout << "error loading file" << std::endl;
		return;
	}
	Sprite.setTexture(Texture);
	Sprite.setPosition(position);
	if (scale == 'L')
	{
		Sprite.setOrigin(51.0f / 2.0f, 46.0f / 2.0f);
	}
	if (scale == 'M')
	{
		Sprite.scale(0.5f, 0.5f);
		Sprite.setOrigin(51.0f / 4.0f, 46.0f / 4.0f);
	}
	if (scale == 'S')
	{
		Sprite.scale(0.25f, 0.25f);
		Sprite.setOrigin(51.0f / 8.0f, 46.0f / 8.0f);
	}
};

Asteroids::Asteroids()
{
	
};

Asteroids::~Asteroids()
{
	for (int i = 0; i < numAsteroids; i++)
	{
		delete asteroids[i];
	}
};
