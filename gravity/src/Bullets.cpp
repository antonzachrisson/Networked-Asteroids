#include "Bullets.hpp"

Bullets::Bullets(sf::Vector2f position_, float rot, bool player)
{
	position = position_;
	shape.setOrigin(1.5f, 5.0f);
	shape.setFillColor(sf::Color::White);
	if (player == false)
	{
		shape.setFillColor(sf::Color::Red);
	}
	shape.setPosition(position_);
	shape.setRotation(rot);
};

Bullets::~Bullets()
{
	for (int i = 0; i < numbullets; i++)
	{
		delete bullets[i];
	}
};