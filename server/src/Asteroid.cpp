#include "Asteroid.hpp"

Asteroids::Asteroids(char scale_, sf::Vector2f position_, sf::Vector2f velocity_)
{
	position = position_;
	velocity = velocity_;
	lives = 2;
	numAsteroids = 0;
	scale = scale_;
	temp = nullptr;
	
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

Asteroids::~Asteroids()
{
	for (int i = 0; i < numAsteroids; i++)
	{
		delete asteroids[i];
	}
};

void Asteroids::move()
{
	for (int i = 0; i < numAsteroids; i++)
	{
		asteroids[i]->Sprite.move(asteroids[i]->velocity);
		asteroids[i]->position = asteroids[i]->Sprite.getPosition();
	}
}

void Asteroids::multiply()
{
	for (int i = 0; i < numAsteroids; i++)
	{
		if (asteroids[i]->lives <= 0)
		{
			std::vector<Asteroids*>::size_type index = numAsteroids;
			sf::Vector2f position_ = asteroids[i]->Sprite.getPosition();
			char m_scale = asteroids[i]->scale;
			temp = asteroids[i];
			asteroids[i] = asteroids[index - 1];
			asteroids[index - 1] = temp;
			asteroids.pop_back();
			numAsteroids--;
			float vectorLength = sqrt((temp->velocity.x * temp->velocity.x) + (temp->velocity.y * temp->velocity.y));
			float angle1 = acos(temp->velocity.x / vectorLength) + 0.52f;
			float angle2 = acos(temp->velocity.x / vectorLength) - 0.52f;
			float velx1 = cos(angle1) * vectorLength;
			float vely1 = sin(angle1) * vectorLength;
			float velx2 = cos(angle2) * vectorLength;
			float vely2 = sin(angle2) * vectorLength;

			if (m_scale == 'L')
			{
				asteroids.push_back(new Asteroids('M', position_, sf::Vector2f(velx1, velx2)));
				asteroids.push_back(new Asteroids('M', position_, sf::Vector2f(velx2, vely2)));
				numAsteroids += 2;
			}
			else if (m_scale == 'M')
			{
				asteroids.push_back(new Asteroids('S', position_, sf::Vector2f(velx1, vely1)));
				asteroids.push_back(new Asteroids('S', position_, sf::Vector2f(velx2, vely2)));
				numAsteroids += 2;
			}
		}
	}
}

int Asteroids::getNumAsteroids()
{
	return numAsteroids;
}

int Asteroids::getLives()
{
	return lives;
}

void Asteroids::addNumAsteroids()
{
	numAsteroids++;
}

void Asteroids::removeLives()
{
	lives--;
}

void Asteroids::addAsteroids()
{
	
	sf::Vector2f position_;
	position_.x = static_cast <float> (rand() % 640 + 100);
	position_.y = static_cast <float> (rand() % 480 + 100);
	sf::Vector2f velocity_;
	velocity_.x = ((float(rand()) / float(RAND_MAX)) * (1.0f - -1.0f)) + -1.0f;
	velocity_.y = ((float(rand()) / float(RAND_MAX)) * (1.0f - -1.0f)) + -1.0f;
	asteroids.push_back(new Asteroids('L', position_, velocity_));
}

void Asteroids::addCollider(int index)
{
	Colliders.push_back(asteroids[index]->Sprite.getGlobalBounds());
}

int Asteroids::asteroidsSize()
{
	return int(asteroids.size());
}

Asteroids* Asteroids::getAsteroid(int index)
{
	return asteroids[index];
}

sf::Rect<float> Asteroids::getCollider(int index)
{
	return Colliders[index];
}

void Asteroids::clearColliders()
{
	Colliders.clear();
}

void Asteroids::update()
{
	multiply();
	move();
}
