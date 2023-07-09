#include "UFO.hpp"

UFO::UFO()
{
	position.x = static_cast <float> (rand () % 640 + 100);
	position.y = static_cast <float> (rand() % 480 + 100);
	velocity.x = ((float(rand()) / float(RAND_MAX)) * (0.7f - -0.7f)) + -0.7f;
	velocity.y = ((float(rand()) / float(RAND_MAX)) * (0.7f - -0.7f)) + -0.7f;
	lives = 3;
	numFramesMove = 0;
	alive = true;
	if (!Texture.loadFromFile("assets/UFO.png"))
	{
		std::cout << "error loading file" << std::endl;
		return;
	}
	Sprite.setTexture(Texture);
	Sprite.setPosition(position);
	Sprite.setOrigin(20.0f, 10.0f);
};

void UFO::changeDirection()
{
	numFramesMove++;
	if (numFramesMove >= rand() % 600 + 120)
	{
		velocity.x = ((float(rand()) / float(RAND_MAX)) * (0.7f - -0.7f)) + -0.7f;
		velocity.y = ((float(rand()) / float(RAND_MAX)) * (0.7f - -0.7f)) + -0.7f;
		numFramesMove = 0;
	}
}

void UFO::move()
{
	Sprite.move(velocity.x, velocity.y);
	sf::Vector2f tempVec = Sprite.getPosition();
	position = tempVec;
}

int UFO::getLives()
{
	return lives;
}

void UFO::removeLives()
{
	lives--;
}

void UFO::reset()
{
	position.x = static_cast <float> (rand() % 640 + 100);
	position.y = static_cast <float> (rand() % 480 + 100);
	velocity.x = ((float(rand()) / float(RAND_MAX)) * (0.7f - -0.7f)) + -0.7f;
	velocity.y = ((float(rand()) / float(RAND_MAX)) * (0.7f - -0.7f)) + -0.7f;
	lives = 3;
	numFramesMove = 0;
	alive = true;
}

void UFO::update(bool m_alive)
{
	if (m_alive)
	{
		changeDirection();
		move();
	}
}