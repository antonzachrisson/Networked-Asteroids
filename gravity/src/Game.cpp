#include "Game.hpp"
#include "input.hpp"
#include <iostream>

Game::Game(keyboard& m_keyboard)
	: m_keyboard(m_keyboard)
{
	bulletsptr = new Bullets(sf::Vector2f(0.0f, 0.0f), 0.0f, true);

	srand((unsigned int)time(nullptr));
	asteroidsptr = new Asteroids();

	numFrames = 0;
	numFramesTP = 0;
	numFramesShoot = 0;
	wave = 0;
	running = false;
	win = false;
	loading = true;
};

Game::~Game()
{
	delete bulletsptr;
	delete asteroidsptr;
	delete otherplayerptr;
}

bool Game::Update(float deltaTime)
{	
	if (loading)
	{
		return true;
	}
	
	if (running)
	{
		asteroidsptr->asteroids.clear();
		bulletsptr->bullets.clear();
		bulletsptr->ufobullets.clear();

		asteroidsptr->numAsteroids = 0;
		for (int i = 0; i < 20; i++)
		{
			if (m_asteroidScale[i] == 'L' || m_asteroidScale[i] == 'M' || m_asteroidScale[i] == 'S')
			{
				asteroidsptr->numAsteroids++;
			}
		}

		bulletsptr->numbullets = 0;
		bulletsptr->ufonumbullets = 0;
		for (int i = 0; i < 20; i++)
		{
			if (m_playerBulletsPos[i][0] != -1 && m_playerBulletsPos[i][1] != -1 && m_playerBulletsPos[i][2] != -1)
			{
				bulletsptr->numbullets++;
			}
		}

		for (int i = 0; i < 5; i++)
		{
			if (m_ufoBulletsPos[i][0] != -1 && m_ufoBulletsPos[i][1] != -1 && m_ufoBulletsPos[i][2] != -1)
			{
				bulletsptr->ufonumbullets++;
			}
		}
		
		for (int i = 0; i < asteroidsptr->numAsteroids; i++)
		{
			asteroidsptr->asteroids.push_back(new Asteroids(m_asteroidScale[i], sf::Vector2f(m_asteroidPos[i][0], m_asteroidPos[i][1])));
		}

		for (int i = 0; i < bulletsptr->numbullets; i++)
		{
			bulletsptr->bullets.push_back(new Bullets(sf::Vector2f( m_playerBulletsPos[i][0], m_playerBulletsPos[i][1]), m_playerBulletsPos[i][2], true));
		}

		for (int i = 0; i < bulletsptr->ufonumbullets; i++)
		{
			bulletsptr->ufobullets.push_back(new Bullets(sf::Vector2f(m_ufoBulletsPos[i][0], m_ufoBulletsPos[i][1]), m_ufoBulletsPos[i][2], false));
		}

		if (otherplayerptr == nullptr)
		{
			if (m_player2Pos[0] != -1 && m_player2Pos[1] != -1 && m_player2Pos[2] != -1)
			{
				otherplayerptr = new OtherPlayer(sf::Vector2f(20.0f, 100.0f));
			}
		}

		if (otherplayerptr != nullptr)
		{
			otherplayerptr->position.x = m_player2Pos[0];
			otherplayerptr->position.y = m_player2Pos[1];
			otherplayerptr->rotation = m_player2Pos[2];
		}

		ufoptr.position.x = m_ufoPos[0];
		ufoptr.position.y = m_ufoPos[1];
		ufoptr.alive = m_ufoAlive;

		guiptr.Score = m_score;
		guiptr.Lives = m_lives;
		guiptr.Wave = m_wave;

		numFramesTP++;
		bool up = m_keyboard.down(sf::Keyboard::Key::I);
		bool left = m_keyboard.down(sf::Keyboard::Key::J);
		bool right = m_keyboard.down(sf::Keyboard::Key::L);
		playerptr.Update(numFramesTP, deltaTime, up, left, right);

		if (otherplayerptr != nullptr)
		{
			otherplayerptr->Update();
		}
		ufoptr.Update();
		guiptr.update();

		worldWrappingGetPos();

		return true;
	}
	if (win)
	{
		winscreen();
	}
	else
	{
		losescreen();
	}
	return true;
}

void Game::Draw(sf::RenderWindow& window)
{
	window.draw(playerptr.Sprite);

	//temp
	if (otherplayerptr != nullptr)
	{
		window.draw(otherplayerptr->Sprite);
	}

	for (int i = 0; i < asteroidsptr->numAsteroids; i++)
	{
		window.draw(asteroidsptr->asteroids[i]->Sprite);
	}
	if (bulletsptr != nullptr)
	{
		for (int i = 0; i < bulletsptr->numbullets; i++)
		{
			window.draw(bulletsptr->bullets[i]->shape);
		}
		for (int i = 0; i < bulletsptr->ufonumbullets; i++)
		{
			window.draw(bulletsptr->ufobullets[i]->shape);
		}
	}
	window.draw(guiptr.score);
	window.draw(guiptr.lives);
	window.draw(wintxt);
	window.draw(losetxt);
	window.draw(guiptr.wave);
	if(ufoptr.alive) window.draw(ufoptr.Sprite);
}

void Game::winscreen()
{
	wintxt.setFont(guiptr.font);
	wintxt.setString("YOU WIN!");
	wintxt.setCharacterSize(30);
	wintxt.setFillColor(sf::Color::White);
	sf::Rect<float> rect = wintxt.getLocalBounds();
	wintxt.setPosition((640.0f - rect.width) / 2, (480.0f - rect.height) / 2);
}

void Game::losescreen()
{
	losetxt.setFont(guiptr.font);
	losetxt.setString("YOU LOSE!");
	losetxt.setCharacterSize(30);
	losetxt.setFillColor(sf::Color::White);
	sf::Rect<float> rect = losetxt.getLocalBounds();
	losetxt.setPosition((640.0f - rect.width) / 2, (480.0f - rect.height) / 2);
}

void Game::worldWrappingGetPos()
{
	playerptr.Sprite.setPosition(worldWrappingSetPos(playerptr.Sprite.getPosition()));
}

sf::Vector2f Game::worldWrappingSetPos(sf::Vector2f objectsPos)
{
	if (objectsPos.x < 0.0f)
	{
		objectsPos.x = 1280.0f;
	}

	if (objectsPos.x > 1280.0f)
	{
		objectsPos.x = 0.0f;
	}

	if (objectsPos.y < 0.0f)
	{
		objectsPos.y = 720.0f;
	}

	if (objectsPos.y > 720.0f)
	{
		objectsPos.y = 0.0f;
	}
	return objectsPos;
}
