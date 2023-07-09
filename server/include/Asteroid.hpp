#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <cmath>

class Asteroids
{
public:
	void move();
	void multiply();
	sf::Vector2f position;
	sf::Vector2f velocity;
	char scale;
	int lives;
	int numAsteroids;
	Asteroids* temp;
	sf::Texture Texture;
	std::vector<Asteroids*> asteroids;
	std::vector<sf::Rect<float>> Colliders;
	Asteroids(char scale_, sf::Vector2f position_, sf::Vector2f velocity_);
	~Asteroids();
	void update();
	int getNumAsteroids();
	int getLives();
	void addNumAsteroids();
	void removeLives();
	void addAsteroids();
	void addCollider(int index);
	sf::Rect<float> getCollider(int index);
	void clearColliders();
	int asteroidsSize();
	Asteroids* getAsteroid(int index);
	sf::Sprite Sprite;
};
