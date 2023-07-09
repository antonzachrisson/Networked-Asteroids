#include "Player.hpp"
#include <iostream>

Player::Player(sf::Vector2f position_)
{
	position = position_;
	velocity.x = 1.0f;
	velocity.y = 0.0f;
	rotation = 90.0f;
	if (!Texture.loadFromFile("assets/Player.png"))
	{
		std::cout << "error loading file" << std::endl;
		return;
	}
	Sprite.setTexture(Texture);
	Sprite.setPosition(position_);
	Sprite.setOrigin(9.5f, 10.0f);
	Sprite.setRotation(rotation);
	srand(static_cast<unsigned int>(time(nullptr)));
};

void Player::setDirection(float deltaTime)
{
	direction = null;
	//0 is I
	if (m_keys_down[0])
	{
		direction = up;
	}
	//1 is J
	if (m_keys_down[1])
	{
		rotation = Sprite.getRotation() - (deltaTime * kPlayerRotationSpeed);
		Sprite.setRotation(rotation);
	}
	//2 is L
	if (m_keys_down[2])
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
	Sprite.move(move);
	position = Sprite.getPosition();
}

void Player::update(float deltaTime)
{
	setDirection(deltaTime);
	move(deltaTime);
	sf::Vector2f tempVec = Sprite.getPosition();
	position = tempVec;
}