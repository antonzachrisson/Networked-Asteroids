#include "Player.hpp"
#include <iostream>
//#include <time.hpp>

Player::Player(float x, float y)
{
	position.x = x;
	position.y = y;
	velocity.x = 1.0f;
	velocity.y = 0.0f;
	rotation = 90.0f;
	if (!Texture.loadFromFile("assets/Player.png"))
	{
		std::cout << "error loading file" << std::endl;
		return;
	}
	Sprite.setTexture(Texture);
	Sprite.setPosition(sf::Vector2f(x, y));
	Sprite.setOrigin(9.5f, 10.0f);
	Sprite.setRotation(90.0f);
};

void Player::setDirection(float deltaTime)
{
	direction = null;
	if (m_up)
	{
		direction = up;
	}
	if (m_left)
	{
		rotation = Sprite.getRotation() - (deltaTime * kPlayerRotationSpeed);
		Sprite.setRotation(rotation);
	}
	if (m_right)
	{
		rotation = Sprite.getRotation() + (deltaTime * kPlayerRotationSpeed);
		Sprite.setRotation(rotation);
	}
}

void Player::move(float deltaTime)
{
	if (direction == null)
	{
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}

	else
	{
		float temprot = Sprite.getRotation() - 90.0f;
		velocity.x = cosf((temprot * (atan(1.0f) * 4.0f)) / 180.0f);
		velocity.y = sinf((temprot * (atan(1.0f) * 4.0f)) / 180.0f);
	}

	float deltaSpeed = deltaTime * kPlayerMoveSpeed;
	sf::Vector2f move = velocity * deltaSpeed;
	Sprite.move(move.x, move.y);
	position = Sprite.getPosition();
}

void Player::Update(int& numFramesTP, float deltaTime, bool up, bool left, bool right)
{
	setDirection(deltaTime);
	move(deltaTime);
	sf::Vector2f tempVec = Sprite.getPosition();
	position = tempVec;
	m_up = up;
	m_left = left;
	m_right = right;
}