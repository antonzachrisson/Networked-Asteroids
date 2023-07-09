// messages.cpp

#include "shared/messages.hpp"

server_info_message::server_info_message()
   : m_type(uint8(message_type::server_info))
{
}

server_info_message::server_info_message(uint32 tick)
   : m_type(uint8(message_type::server_info))
   , m_tick(tick)
{
}

server_info_message::server_info_message(uint32 tick, bool keys_down[])
	: m_type(uint8(message_type::server_info))
	, m_tick(tick)
{
	for (int i = 0; i < 4; i++)
	{
		m_keys_down[i] = keys_down[i];
	}
}

server_info_message::server_info_message(uint32 tick, uint32 sequence, uint32 acknowledge, float asteroidPos[][2], char asteroidScale[], float playerBulletsPos[][3], float ufoBulletsPos[][3], float playerPos[], float player2Pos[], float ufoPos[], bool ufoAlive, int score, int lives, int wave, bool running, bool win, bool loading)
	: m_type(uint8(message_type::server_info))
	, m_tick(tick)
{	
	m_sequence = sequence;
	m_acknowledge = acknowledge;

	for (int i = 0; i < 20; i++)
	{
		m_asteroidPos[i][0] = asteroidPos[i][0];
		m_asteroidPos[i][1] = asteroidPos[i][1];
		m_asteroidScale[i] = asteroidScale[i];
	}
	
	for (int i = 0; i < 20; i++)
	{
		m_playerBulletsPos[i][0] = playerBulletsPos[i][0];
		m_playerBulletsPos[i][1] = playerBulletsPos[i][1];
    	m_playerBulletsPos[i][2] = playerBulletsPos[i][2];
	}

	for (int i = 0; i < 5; i++)
	{
		m_ufoBulletsPos[i][0] = ufoBulletsPos[i][0];
		m_ufoBulletsPos[i][1] = ufoBulletsPos[i][1];
		m_ufoBulletsPos[i][2] = ufoBulletsPos[i][2];
	}
	

	m_playerPos[0] = playerPos[0];
	m_playerPos[1] = playerPos[1];
	m_playerPos[2] = playerPos[2];
	m_player2Pos[0] = player2Pos[0];
	m_player2Pos[1] = player2Pos[1];
	m_player2Pos[2] = player2Pos[2];
	m_ufoPos[0] = ufoPos[0];
	m_ufoPos[1] = ufoPos[1];
	m_ufoAlive = ufoAlive;
	m_score = score;
	m_lives = lives;
	m_wave = wave;
	m_running = running;
	m_win = win;
	m_loading = loading;
}

bool server_info_message::read(byte_stream_reader &reader)
{
   return serialize(reader);
}

bool server_info_message::write(byte_stream_writer &writer)
{
   return serialize(writer);
}
