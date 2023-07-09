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
	Player(float x, float y);
	void setDirection(float deltaTime);
	void move(float deltaTime);
	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation;
	bool m_up{ false };
	bool m_left{ false };
	bool m_right{ false };
	sf::Texture Texture;
	sf::Sprite Sprite;
	Direction direction;
	void Update(int& numFramesTP, float deltaTime, bool up, bool left, bool right);
};
