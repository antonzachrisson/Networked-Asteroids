#pragma once

#include "Game.hpp"
#include "client.hpp"
#include <initializer_list>
#include <SFML/Graphics.hpp>


class Interpolator final
{
public:
	Interpolator() = default;

	bool InterpolateEntities(sf::Time current_time, Game* game);
	bool add_packet(server_info_message msg, sf::Time time);
	float dist(float pos1[], float pos2[]);

private:
	struct packet
	{
		packet() {};
		packet(server_info_message msg, sf::Time time)
		{
			m_msg = msg;
			m_index = msg.m_sequence;
			m_time = time.asSeconds();
		}

		int m_index{ -1 };
		server_info_message m_msg;
		float m_time{ 0.0f };
	};
	int packetsSize{ 10 };
	packet packets[10];
	int packet_override_index{ 0 };
};