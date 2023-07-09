#include "Bullets.hpp"
#include <iostream>

Bullets::Bullets(sf::Vector2f position_, float m_rotation, bool player)
{
	position = position_;
	rotation = m_rotation;
	lifetime = 180;
	shape = sf::RectangleShape(sf::Vector2f(3.0f, 10.0f));
	shape.setOrigin(1.5f, 5.0f);
	shape.setFillColor(sf::Color::White);
	if (player == false)
	{
		shape.setFillColor(sf::Color::Red);
	}
	shape.setPosition(position_);
	shape.setRotation(rotation);
	velocity.y = -(cosf(rotation / (180.0f / (atanf(1)*4))) * 3.0f);
	velocity.x = sinf(rotation / (180.0f / (atanf(1) * 4))) * 3.0f;
	temp = nullptr;
	numFrames = 0;
};

Bullets::~Bullets()
{
	for (int i = 0; i < numbullets; i++)
	{
		delete bullets[i];
	}
};

void Bullets::move()
{
	for (int i = 0; i < numbullets; i++)
	{
		bullets[i]->shape.move(bullets[i]->velocity);
		bullets[i]->position = bullets[i]->shape.getPosition();
		bullets[i]->numFrames++;
		
		if (bullets[i]->numFrames >= bullets[i]->lifetime)
		{
			std::vector<Bullets*>::size_type index = numbullets;
			temp = bullets[i];
			bullets[i] = bullets[index - 1];
			bullets[index - 1] = temp;
			bullets.pop_back();
			numbullets--;
		}
	}

	for (int i = 0; i < ufonumbullets; i++)
	{
		ufobullets[i]->shape.move(ufobullets[i]->velocity);
		ufobullets[i]->position = ufobullets[i]->shape.getPosition();
		ufobullets[i]->numFrames++;

		if (ufobullets[i]->numFrames >= ufobullets[i]->lifetime)
		{
			std::vector<Bullets*>::size_type index = ufonumbullets;
			temp = ufobullets[i];
			ufobullets[i] = ufobullets[index - 1];
			ufobullets[index - 1] = temp;
			ufobullets.pop_back();
			ufonumbullets--;
		}
	}
}

void Bullets::addPlayerBullet(sf::Vector2f position_, float m_rotation_)
{
	bullets.push_back(new Bullets(position_, m_rotation_, true));
	numbullets++;
}

void Bullets::addUfoBullet(float position_[], float m_rotation_)
{
	ufobullets.push_back(new Bullets(sf::Vector2f(position_[0], position_[1]), m_rotation_, false));
	ufonumbullets++;
}

void Bullets::clearColliders(char type)
{
	if (type == 'P') Colliders.clear();
	if (type == 'U') ufoColliders.clear();
}

void Bullets::removeBullet(char type, int index)
{
	if (type == 'U')
	{
		std::vector<Bullets*>::size_type index_ = ufonumbullets;
		temp = ufobullets[index];
		ufobullets[index] = ufobullets[index_ - 1];
		ufobullets[index_ - 1] = temp;
		ufobullets.pop_back();
		ufonumbullets--;
	}
	if (type == 'P')
	{
		std::vector<Bullets*>::size_type index_ = numbullets;
		temp = bullets[index];
		bullets[index] = bullets[index_ - 1];
		bullets[index_ - 1] = temp;
		bullets.pop_back();
		numbullets--;
	}
}

sf::Rect<float> Bullets::getColliders(int index, char type)
{
	if (type == 'P') return Colliders[index];
	else return ufoColliders[index];
}

int Bullets::getNumBullets(char type)
{
	if (type == 'P') return numbullets;
	else return ufonumbullets;
}

void Bullets::addColliders(int index, char type)
{
	if (type == 'P') Colliders.push_back(bullets[index]->shape.getGlobalBounds());
	if (type == 'U') ufoColliders.push_back(ufobullets[index]->shape.getGlobalBounds());
}

Bullets* Bullets::getBullet(int index, char type)
{
	if (type == 'P') return bullets[index];
	else return ufobullets[index];
}

void Bullets::update()
{
	move();
}