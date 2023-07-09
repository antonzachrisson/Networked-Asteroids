#include "GUI.hpp"

GUI::GUI()
{
	Score = 0;
	Lives = 4;
	if (!font.loadFromFile("assets/Bright Star.ttf"))
	{
		std::cout << "error loading file" << std::endl;
		return;
	}
	score.setFont(font);
	score.setString(std::to_string(Score));
	score.setCharacterSize(20);
	score.setFillColor(sf::Color::White);
	lives.setFont(font);
	lives.setCharacterSize(15);
	lives.setFillColor(sf::Color::Green);
	lives.setPosition(0.0f, 30.0f);
	wave.setFont(font);
	wave.setCharacterSize(15);
	wave.setFillColor(sf::Color::White);
	sf::Rect<float> rect = wave.getLocalBounds();
	wave.setPosition((640.0f - rect.width) / 2, 20.0f);
	strWave = "WAVE  ";
};

void GUI::update()
{
	score.setString(std::to_string(Score));
	strLives.clear();
	for (int i = 0; i < Lives; i++)
	{
		strLives.push_back('A');
	}
	lives.setString(strLives);

	setWave(Wave);
}

void GUI::setWave(int wave_)
{
	if (wave_ == 1)
	{
		strWave.pop_back();
		strWave.push_back('1');
	}
	else
	{
		strWave.pop_back();
		strWave.push_back((char)(48 + wave_));
	}
	wave.setString(strWave);
	sf::Rect<float> rect = wave.getLocalBounds();
	wave.setPosition((640.0f - rect.width) / 2, 20.0f);
}