#pragma once

#include "Game.hpp"
#include "client.hpp"
#include <initializer_list>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class Inputinator final
{
public:

	Inputinator() = default;
	void addPrediction(uint32 tick, sf::Vector2f position, bool up, bool left, bool right, float rot);
	void addInputPrediction(uint32 tick, bool up, bool left, bool right);

	struct prediction
	{
		prediction() {};
		prediction(uint32 tick, sf::Vector2f position, bool up, bool left, bool right, float rot)
		{
			m_tick = tick;
			m_position = position;
			m_up = up;
			m_left = left;
			m_right = right;
			m_rotation = rot;
		}

		uint32 m_tick{ 0 };
		bool m_up{ false };
		bool m_left{ false };
		bool m_right{ false };
		sf::Vector2f m_position;
		float m_rotation;
	};

	struct inputPrediction
	{
		inputPrediction() {};
		inputPrediction(uint32 tick, bool up, bool left, bool right)
		{
			m_tick = tick;
			m_up = up;
			m_left = left;
			m_right = right;
		}

		uint32 m_tick{ 0 };
		bool m_up{ false };
		bool m_left{ false };
		bool m_right{ false };
	};

	std::vector<prediction> m_predictions;
	std::vector<inputPrediction> m_inputPredictions;
	void CheckOffset(server_info_message msg, Game* game);
	const float simRate{ 1.0f / 60.0f };
};