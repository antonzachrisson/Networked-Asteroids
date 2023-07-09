#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

constexpr float kPlayerMoveSpeed = 200.0f;
constexpr float kPlayerRotationSpeed = 250.0f;

enum Direction {
	up,
	null,
};

class Player
{
public:
	void setDirection(float deltaTime);
	void move(float deltaTime);
	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation;
	sf::Texture Texture;
	Direction direction;
	Player(sf::Vector2f position_);
	void update(float deltaTime);
	sf::Sprite Sprite;
	bool m_keys_down[4];
};
