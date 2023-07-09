#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Asteroid.hpp"
#include "Bullets.hpp"
#include "GUI.hpp"
#include "Player.hpp"
#include "UFO.hpp"
#include <iostream>

class Game
{
	void shoot();
	void ufoShoot(bool alive);
	void worldWrappingGetPos();
	void collision();
	void SetWave();
	void GetWave();
	void checkPlayerLives();
	void winscreen();
	void losescreen();
	sf::Vector2f worldWrappingSetPos(sf::Vector2f objectsPos);
	sf::Text wintxt;
	sf::Text losetxt;
	Player playerptr = Player(sf::Vector2f(20.0f, 20.0f));
	Bullets* bulletsptr = nullptr;
	Asteroids* asteroidsptr = nullptr;
	GUI guiptr;
	UFO ufoptr;
	int numFrames;
	int numFramesTP;
	int numFramesShoot;
	int wave;
	bool running;
	bool win;
	bool loading;
	float elapsedLoading;
public:
	Game(unsigned int& screenWidth, unsigned int& screenHeight, const char*& title);
	~Game();
	bool Update(float deltaTime);
	void Draw(sf::RenderWindow& window);
	void addPlayer();
	float distance(float x1, float y1, float x2, float y2);
	bool m_keys_down[4];
	bool m_keys_down2[4];
	float m_asteroidPos[20][2];
	char m_asteroidScale[20];
	float m_playerBulletsPos[20][3];
	float m_ufoBulletsPos[5][3];
	float m_playerPos[3];
	float m_player2Pos[3];
	float m_ufoPos[2];
	bool m_ufoAlive;
	int m_score;
	int m_lives;
	int m_wave;
	bool m_running;
	bool m_win;
	bool m_loading;
	Player* player2ptr = nullptr;
};