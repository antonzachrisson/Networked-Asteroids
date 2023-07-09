#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Asteroid.hpp"
#include "Bullets.hpp"
#include "GUI.hpp"
#include "Player.hpp"
#include "OtherPlayer.hpp"
#include "UFO.hpp"


struct keyboard;

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
		
	OtherPlayer* otherplayerptr = nullptr;

	Bullets* bulletsptr = nullptr;
	Asteroids* asteroidsptr = nullptr;
	GUI guiptr;
	UFO ufoptr;
	int numFrames;
	int numFramesTP;
	int numFramesShoot;
	int wave;
	keyboard& m_keyboard;
public:
	Game(keyboard &m_keyboard);
	~Game();
	bool Update(float deltaTime);
	void Draw(sf::RenderWindow& window);

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
	bool running;
	bool win;
	bool loading;
	Player playerptr = Player(20.0f, 20.0f);
};