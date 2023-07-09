#include "Game.hpp"

Game::Game(unsigned int &screenWidth, unsigned int &screenHeight, const char* &title)
{
	screenWidth = 640;
	screenHeight = 480;
	title = "Asteroids";
	bulletsptr = new Bullets(sf::Vector2f(0.0f, 0.0f), 0.0f, true);

	srand((unsigned int)time(nullptr));
	sf::Vector2f position;
	position.x = static_cast <float> (rand() % 640 + 100);
	position.y = static_cast <float> (rand() % 480 + 100);
	sf::Vector2f velocity;
	velocity.x = ((float(rand()) / float(RAND_MAX)) * (1.0f - -1.0f)) + -1.0f;
	velocity.y = ((float(rand()) / float(RAND_MAX)) * (1.0f - -1.0f)) + -1.0f;
	asteroidsptr = new Asteroids('L', position, velocity);

	numFrames = 0;
	numFramesTP = 0;
	numFramesShoot = 0;
	wave = 0;
	running = false;
	win = false;
	loading = true;
	elapsedLoading = 0.0f;
};

Game::~Game()
{
	delete bulletsptr;
	delete asteroidsptr;
	delete player2ptr;
}

void Game::shoot()
{
	//3 is spacebar
	if (playerptr.m_keys_down[3] && numFrames > 30)
	{
		sf::Vector2f pos = playerptr.Sprite.getPosition();
		float rot = playerptr.Sprite.getRotation();
		bulletsptr->addPlayerBullet(pos, rot);
		numFrames = 0;
	}

	if (player2ptr != nullptr)
	{
		if (player2ptr->m_keys_down[3] && numFrames > 30)
		{
			sf::Vector2f pos = player2ptr->Sprite.getPosition();
			float rot = player2ptr->Sprite.getRotation();
			bulletsptr->addPlayerBullet(pos, rot);
			numFrames = 0;
		}
	}
}

bool Game::Update(float deltaTime)
{
	m_running = running;
	m_win = win;
	m_loading = loading;

	if (loading)
	{
		elapsedLoading += deltaTime;
		if (elapsedLoading >= 1.0f)
		{
			running = true;
			elapsedLoading = 0.0f;
			loading = false;
		}
		return true;
	}

	if (running)
	{		
		numFrames += 1;
		numFramesTP += 1;
		for (int i = 0; i < 4; i++)
		{
			playerptr.m_keys_down[i] = m_keys_down[i];
		}
		if (player2ptr != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				player2ptr->m_keys_down[i] = m_keys_down2[i];
			}
		}
		GetWave();
		SetWave();
		playerptr.update(deltaTime);
		if (player2ptr != nullptr)
		{
			player2ptr->update(deltaTime);
		}
		
		asteroidsptr->update();
		shoot();
		ufoShoot(ufoptr.alive);
		worldWrappingGetPos();
		if (bulletsptr != nullptr)
		{
			bulletsptr->update();
		}
		ufoptr.update(ufoptr.alive);
		collision();
		guiptr.update(asteroidsptr);
		checkPlayerLives();

		for (int i = 0; i < 20; i++)
		{
			m_asteroidPos[i][0] = -1;
			m_asteroidPos[i][1] = -1;
			m_asteroidScale[i] = NULL;
		}

		for (int i = 0; i < 20; i++)
		{
			m_playerBulletsPos[i][0] = -1;
			m_playerBulletsPos[i][1] = -1;
			m_playerBulletsPos[i][2] = -1;
		}

		for (int i = 0; i < 5; i++)
		{
			m_ufoBulletsPos[i][0] = -1;
			m_ufoBulletsPos[i][1] = -1;
			m_ufoBulletsPos[i][2] = -1;
		}
		
		for (int i = 0; i < asteroidsptr->getNumAsteroids(); i++)
		{
			m_asteroidPos[i][0] = asteroidsptr->asteroids[i]->position.x;
			m_asteroidPos[i][1] = asteroidsptr->asteroids[i]->position.y;
			
			m_asteroidScale[i] = asteroidsptr->asteroids[i]->scale;
		}

		if (bulletsptr != nullptr)
		{
			for (int i = 0; i < bulletsptr->numbullets; i++)
			{
				if (i <= 20)
				{
					m_playerBulletsPos[i][0] = bulletsptr->bullets[i]->position.x;
					m_playerBulletsPos[i][1] = bulletsptr->bullets[i]->position.y;
					m_playerBulletsPos[i][2] = bulletsptr->bullets[i]->rotation;
				}
			}
			for (int i = 0; i < bulletsptr->ufonumbullets; i++)
			{
				if (i <= 5)
				{
					m_ufoBulletsPos[i][0] = bulletsptr->ufobullets[i]->position.x;
					m_ufoBulletsPos[i][1] = bulletsptr->ufobullets[i]->position.y;
					m_ufoBulletsPos[i][2] = bulletsptr->ufobullets[i]->rotation;
				}
			}
		}

		m_playerPos[0] = playerptr.position.x;
		m_playerPos[1] = playerptr.position.y;
		m_playerPos[2] = playerptr.rotation;

		m_player2Pos[0] = -1;
		m_player2Pos[1] = -1;
		m_player2Pos[2] = -1;
		
		//temp
		if (player2ptr != nullptr)
		{
			m_player2Pos[0] = player2ptr->position.x;
			m_player2Pos[1] = player2ptr->position.y;
			m_player2Pos[2] = player2ptr->rotation;
		}

		m_ufoPos[0] = ufoptr.position.x;
		m_ufoPos[1] = ufoptr.position.y;
		m_ufoAlive = ufoptr.alive;
		m_score = guiptr.Score;
		m_lives = guiptr.Lives;
		m_wave = guiptr.Wave;

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
	if (player2ptr != nullptr)
	{
		window.draw(player2ptr->Sprite);
	}
	for (int i = 0; i < asteroidsptr->getNumAsteroids(); i++)
	{
		window.draw(asteroidsptr->getAsteroid(i)->Sprite);
	}
	if (bulletsptr != nullptr)
	{
		for (int i = 0; i < bulletsptr->getNumBullets('P'); i++)
		{
			window.draw(bulletsptr->getBullet(i, 'P')->shape);
		}
		for (int i = 0; i < bulletsptr->getNumBullets('U'); i++)
		{
			window.draw(bulletsptr->getBullet(i, 'U')->shape);
		}
	}
	window.draw(guiptr.score);
	window.draw(guiptr.lives);
	window.draw(wintxt);
	window.draw(losetxt);
	window.draw(guiptr.wave);
	if(ufoptr.alive) window.draw(ufoptr.Sprite);
}

void Game::worldWrappingGetPos()
{
	playerptr.Sprite.setPosition(worldWrappingSetPos(playerptr.Sprite.getPosition()));
	if (player2ptr != nullptr)
	{
		player2ptr->Sprite.setPosition(worldWrappingSetPos(player2ptr->Sprite.getPosition()));
	}
	ufoptr.Sprite.setPosition(worldWrappingSetPos(ufoptr.Sprite.getPosition()));
	for (int i = 0; i < asteroidsptr->getNumAsteroids(); i++)
	{
		asteroidsptr->getAsteroid(i)->Sprite.setPosition(worldWrappingSetPos(asteroidsptr->getAsteroid(i)->Sprite.getPosition()));
	}

	for (int i = 0; i < bulletsptr->getNumBullets('P'); i++)
	{
		bulletsptr->getBullet(i, 'P')->shape.setPosition(worldWrappingSetPos(bulletsptr->getBullet(i, 'P')->shape.getPosition()));
	}
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

void Game::collision()
{
	sf::Rect<float> PlayerRect = playerptr.Sprite.getGlobalBounds();
	sf::Rect<float> Player2Rect;
	if (player2ptr != nullptr)
	{
		Player2Rect = player2ptr->Sprite.getGlobalBounds();
	}
	sf::Rect<float> ufoRect = ufoptr.Sprite.getGlobalBounds();
	for (int i = 0; i < asteroidsptr->getNumAsteroids(); i++)
	{
		asteroidsptr->addCollider(i);
	}
	for (int i = 0; i < bulletsptr->getNumBullets('P'); i++)
	{
		bulletsptr->addColliders(i, 'P');
	}
	for (int i = 0; i < bulletsptr->getNumBullets('U'); i++)
	{
		bulletsptr->addColliders(i, 'U');
	}
	if (ufoRect.intersects(PlayerRect) && ufoptr.alive)
	{
		guiptr.removeLives();
		playerptr.Sprite.setPosition(20.0f, 20.0f);
		playerptr.Sprite.setRotation(90.0f);
	}

	if (player2ptr != nullptr)
	{
		if (ufoRect.intersects(Player2Rect) && ufoptr.alive)
		{
			guiptr.removeLives();
			player2ptr->Sprite.setPosition(20.0f, 100.0f);
			player2ptr->Sprite.setRotation(90.0f);
		}
	}
	
	for (int i = 0; i < bulletsptr->getNumBullets('P'); i++)
	{
		if (bulletsptr->getColliders(i, 'P').intersects(ufoRect) && ufoptr.alive)
		{
			ufoptr.removeLives();
			numFramesShoot = 0;
			bulletsptr->removeBullet('P', i);
			if (ufoptr.getLives() <= 0)
			{
				ufoptr.alive = false;
			}
		}
	}
	for (int i = 0; i < asteroidsptr->getNumAsteroids(); i++)
	{
		if (asteroidsptr->getCollider(i).intersects(PlayerRect))
		{
			guiptr.removeLives();
			playerptr.Sprite.setPosition(20.0f, 20.0f);
			playerptr.Sprite.setRotation(90.0f);
		}

		if (player2ptr != nullptr)
		{
			if (asteroidsptr->getCollider(i).intersects(Player2Rect))
			{
				guiptr.removeLives();
				player2ptr->Sprite.setPosition(20.0f, 100.0f);
				player2ptr->Sprite.setRotation(90.0f);
			}
		}
	}
	for (int i = 0; i < bulletsptr->getNumBullets('P'); i++)
	{
		for (int j = 0; j < asteroidsptr->getNumAsteroids(); j++)
		{
			if (asteroidsptr->getCollider(j).intersects(bulletsptr->getColliders(i, 'P')))
			{
				asteroidsptr->getAsteroid(j)->removeLives();
				bulletsptr->removeBullet('P', i);
			}
		}
	}
	for (int i = 0; i < bulletsptr->getNumBullets('U'); i++)
	{
		if (bulletsptr->getColliders(i, 'U').intersects(PlayerRect) && ufoptr.alive)
		{
			guiptr.removeLives();
			playerptr.Sprite.setPosition(20.0f, 20.0f);
			playerptr.Sprite.setRotation(90.0f);
			bulletsptr->removeBullet('U', i);
		}

		if (player2ptr != nullptr)
		{
			if (bulletsptr->getColliders(i, 'U').intersects(Player2Rect) && ufoptr.alive)
			{
				guiptr.removeLives();
				player2ptr->Sprite.setPosition(20.0f, 100.0f);
				player2ptr->Sprite.setRotation(90.0f);
				bulletsptr->removeBullet('U', i);
			}
		}
	}
	asteroidsptr->clearColliders();
	bulletsptr->clearColliders('P');
	bulletsptr->clearColliders('U');
}

void Game::GetWave()
{
	if (asteroidsptr->asteroidsSize() == 0 && wave == 5)
	{
		win = true;
		running = false;
	}
	else if (asteroidsptr->asteroidsSize() == 0)
	{
		wave++;
		guiptr.setWave(wave);
		ufoptr.reset();
	}
}

void Game::SetWave()
{
	if (asteroidsptr->asteroidsSize() == 0)
	{
		playerptr.Sprite.setPosition(20.0f, 20.0f);
		playerptr.Sprite.setRotation(90.0f);
		if (player2ptr != nullptr)
		{
			player2ptr->Sprite.setPosition(20.0f, 100.0f);
			player2ptr->Sprite.setRotation(90.0f);
		}
		srand((unsigned int)time(nullptr));
		for (int i = 0; i < wave; i++)
		{
			asteroidsptr->addAsteroids();
			asteroidsptr->addNumAsteroids();
		}
	}
}

void Game::checkPlayerLives()
{
	if (guiptr.getLives() <= 0)
	{
		win = false;
		running = false;
	}
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

void Game::ufoShoot(bool alive)
{
	if(alive) numFramesShoot++;
	if (numFramesShoot >= rand() % 1200 + 240 && alive)
	{
		float ufopos[2] = { ufoptr.position.x, ufoptr.position.y };
		float playerpos[2] = { playerptr.position.x, playerptr.position.y };

		if (player2ptr != nullptr)
		{
			if (distance(ufoptr.position.x, ufoptr.position.y, playerptr.position.x, playerptr.position.y) > distance(ufoptr.position.x, ufoptr.position.y, player2ptr->position.x, player2ptr->position.y))
			{
				playerpos[0] = player2ptr->position.x;
				playerpos[1] = player2ptr->position.y;
			}
		}

		float x = ufopos[0] - playerpos[0];
		float y = ufopos[1] - playerpos[1];
		float hyp = sqrt((x * x) + (y * y));
		float rot;
		if (y >= 0)
		{
			rot = (acosf(x / hyp) * (180.0f / (atanf(1) * 4))) - 90.0f;
		}
		else
		{
			rot = -(acosf(x / hyp) * (180.0f / (atanf(1) * 4))) - 90.0f;
		}
		bulletsptr->addUfoBullet(ufopos, rot);
		numFramesShoot = 0;
	}
}

void Game::addPlayer()
{
	if (player2ptr == nullptr)
	{
		player2ptr = new Player(sf::Vector2f(20.0f, 100.0f));
	}
}

float Game::distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2 - x1, 2.0f) + pow(y2 - y1, 2.0f) * 1.0f);
}
