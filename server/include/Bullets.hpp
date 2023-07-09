#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <cmath>

class Bullets
{
public:
	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation;
	Bullets* temp;
	std::vector<Bullets*> bullets;
	std::vector<Bullets*> ufobullets;
	int numbullets = 0;
	int ufonumbullets = 0;
	int numFrames;
	int lifetime;
	std::vector<sf::Rect<float>> Colliders;
	std::vector<sf::Rect<float>> ufoColliders;
	void move();
	Bullets(sf::Vector2f position_, float m_rotation, bool player);
	~Bullets();
	void update();
	void removeBullet(char type, int index);
	void addUfoBullet(float position_[], float m_rotation_);
	void addPlayerBullet(sf::Vector2f position_, float m_rotation_);
	sf::Rect<float> getColliders(int index, char type);
	void addColliders(int index, char type);
	void clearColliders(char type);
	int getNumBullets(char type);
	Bullets* getBullet(int index, char type);
	sf::RectangleShape shape;
};
