#include "Interpolator.hpp"

bool Interpolator::InterpolateEntities(sf::Time current_time, Game* game)
{
	if (packets[0].m_index == -1)
		return false;

	float time = current_time.asSeconds() - 0.2f;

	packet before;
	packet after;

	int y = 0;
	while (packets[y].m_time < time)
	{
		y++;
		if (y == packetsSize - 1)
			break;
	}	
	if (y < 1)
		before = packets[9];
	else
		before = packets[y - 1];
	after = packets[y];

	float alpha = (time - before.m_time) / (after.m_time - before.m_time);

	for (int i = 0; i < 20; i++)
	{
		if (dist(before.m_msg.m_asteroidPos[i], after.m_msg.m_asteroidPos[i]) > 100.0f)
		{
			game->m_asteroidPos[i][0] = after.m_msg.m_asteroidPos[i][0];
			game->m_asteroidPos[i][1] = after.m_msg.m_asteroidPos[i][1];
		}
		
		else
		{
			game->m_asteroidPos[i][0] = before.m_msg.m_asteroidPos[i][0] + (after.m_msg.m_asteroidPos[i][0] - before.m_msg.m_asteroidPos[i][0]) * alpha;
			game->m_asteroidPos[i][1] = before.m_msg.m_asteroidPos[i][1] + (after.m_msg.m_asteroidPos[i][1] - before.m_msg.m_asteroidPos[i][1]) * alpha;
		}
	}

	for (int i = 0; i < 20; i++)
	{
		if (dist(before.m_msg.m_playerBulletsPos[i], after.m_msg.m_playerBulletsPos[i]) > 100.0f)
		{
			game->m_playerBulletsPos[i][0] = after.m_msg.m_playerBulletsPos[i][0];
			game->m_playerBulletsPos[i][1] = after.m_msg.m_playerBulletsPos[i][1];
			game->m_playerBulletsPos[i][2] = after.m_msg.m_playerBulletsPos[i][2];
		}
		
		else
		{
			game->m_playerBulletsPos[i][0] = before.m_msg.m_playerBulletsPos[i][0] + (after.m_msg.m_playerBulletsPos[i][0] - before.m_msg.m_playerBulletsPos[i][0]) * alpha;
			game->m_playerBulletsPos[i][1] = before.m_msg.m_playerBulletsPos[i][1] + (after.m_msg.m_playerBulletsPos[i][1] - before.m_msg.m_playerBulletsPos[i][1]) * alpha;
			game->m_playerBulletsPos[i][2] = before.m_msg.m_playerBulletsPos[i][2] + (after.m_msg.m_playerBulletsPos[i][2] - before.m_msg.m_playerBulletsPos[i][2]) * alpha;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (dist(before.m_msg.m_ufoBulletsPos[i], after.m_msg.m_ufoBulletsPos[i]) > 100.0f)
		{
			game->m_ufoBulletsPos[i][0] = after.m_msg.m_ufoBulletsPos[i][0];
			game->m_ufoBulletsPos[i][1] = after.m_msg.m_ufoBulletsPos[i][1];
			game->m_ufoBulletsPos[i][2] = after.m_msg.m_ufoBulletsPos[i][2];
		}
		
		else
		{
			game->m_ufoBulletsPos[i][0] = before.m_msg.m_ufoBulletsPos[i][0] + (after.m_msg.m_ufoBulletsPos[i][0] - before.m_msg.m_ufoBulletsPos[i][0]) * alpha;
			game->m_ufoBulletsPos[i][1] = before.m_msg.m_ufoBulletsPos[i][1] + (after.m_msg.m_ufoBulletsPos[i][1] - before.m_msg.m_ufoBulletsPos[i][1]) * alpha;
			game->m_ufoBulletsPos[i][2] = before.m_msg.m_ufoBulletsPos[i][2] + (after.m_msg.m_ufoBulletsPos[i][2] - before.m_msg.m_ufoBulletsPos[i][2]) * alpha;
		}
	}


	if (dist(before.m_msg.m_player2Pos, after.m_msg.m_player2Pos) > 100.0f)
	{
		game->m_player2Pos[0] = after.m_msg.m_player2Pos[0];
		game->m_player2Pos[1] = after.m_msg.m_player2Pos[1];
	}

	else
	{
		game->m_player2Pos[0] = before.m_msg.m_player2Pos[0] + (after.m_msg.m_player2Pos[0] - before.m_msg.m_player2Pos[0]) * alpha;
		game->m_player2Pos[1] = before.m_msg.m_player2Pos[1] + (after.m_msg.m_player2Pos[1] - before.m_msg.m_player2Pos[1]) * alpha;
	}

	if (abs(after.m_msg.m_player2Pos[2] - before.m_msg.m_player2Pos[2]) > 100.0f)
	{
		game->m_player2Pos[2] = after.m_msg.m_player2Pos[2];
	}

	else
	{
	    game->m_player2Pos[2] = before.m_msg.m_player2Pos[2] + (after.m_msg.m_player2Pos[2] - before.m_msg.m_player2Pos[2]) * alpha;
	}

	game->m_playerPos[0] = after.m_msg.m_playerPos[0];
	game->m_playerPos[1] = after.m_msg.m_playerPos[1];
	game->m_playerPos[2] = after.m_msg.m_playerPos[2];

	if (dist(before.m_msg.m_ufoPos, after.m_msg.m_ufoPos) > 100.0f)
	{
		game->m_ufoPos[0] = after.m_msg.m_ufoPos[0];
		game->m_ufoPos[1] = after.m_msg.m_ufoPos[1];
	}

	else
	{
		game->m_ufoPos[0] = before.m_msg.m_ufoPos[0] + (after.m_msg.m_ufoPos[0] - before.m_msg.m_ufoPos[0]) * alpha;
		game->m_ufoPos[1] = before.m_msg.m_ufoPos[1] + (after.m_msg.m_ufoPos[1] - before.m_msg.m_ufoPos[1]) * alpha;
	}

	return true;
}

bool Interpolator::add_packet(server_info_message msg, sf::Time time)
{
	bool found_packet{ false };

	for (int i = 0; i < 10; i++)
	{
		if (packets[i].m_index == -1)
		{
			found_packet = true;
			packets[i].m_index = msg.m_sequence;
			packets[i].m_msg = msg;
			packets[i].m_time = time.asSeconds();
			break;
		}
	}

	if (found_packet == false)
	{
		packets[packet_override_index].m_index = -1;
		packet_override_index++;
		if (packet_override_index >= 10)
			packet_override_index = 0;
		add_packet(msg, time);
	}

	return true;
}

float Interpolator::dist(float pos1[], float pos2[])
{
	return sqrt(pow(pos1[0] - pos2[0], 2.0f) + pow(pos1[1] - pos2[1], 2.0f));
}