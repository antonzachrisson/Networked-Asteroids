// messages.hpp

#pragma once

#include <shared/byte_stream.hpp>
#include <vector>

enum class message_type {
   server_info,
   find_server,
};

struct server_info_message {
   server_info_message();
   server_info_message(uint32 tick);
   server_info_message(uint32 tick, bool keys_down[]);
   server_info_message(uint32 tick, uint32 sequence, uint32 acknowledge, float asteroidPos[][2], char asteroidScalep[], float playerBulletsPos[][3], float ufoBulletsPos[][3], float playerPos[], float player2Pos[], float ufoPos[], bool ufoAlive, int score, int lives, int wave, bool running, bool win, bool loading);

   bool read(byte_stream_reader &reader);
   bool write(byte_stream_writer &writer);

   template <typename S>
   bool serialize(S &stream)
   {
      if (!stream.serialize(m_type)) { return false; }
      if (!stream.serialize(m_tick)) { return false; }
      if (!stream.serialize(m_sequence)) { return false; }
      if (!stream.serialize(m_acknowledge)) { return false; }

      for (int i = 0; i < 4; i++)
      {
          if (!stream.serialize(m_keys_down[i])) { return false; }
      }

      for (int i = 0; i < 20; i++)
      {
          if (!stream.serialize(m_asteroidPos[i][0])) { return false; }
          if (!stream.serialize(m_asteroidPos[i][1])) { return false; }
          if (!stream.serialize(m_asteroidScale[i])) { return false; }
      }
      
      for (int i = 0; i < 20; i++)
      {
          if (!stream.serialize(m_playerBulletsPos[i][0])) { return false; }
          if (!stream.serialize(m_playerBulletsPos[i][1])) { return false; }
          if (!stream.serialize(m_playerBulletsPos[i][2])) { return false; }
      }

      for (int i = 0; i < 5; i++)
      {
          if (!stream.serialize(m_ufoBulletsPos[i][0])) { return false; }
          if (!stream.serialize(m_ufoBulletsPos[i][1])) { return false; }
          if (!stream.serialize(m_ufoBulletsPos[i][2])) { return false; }
      }
      
      if (!stream.serialize(m_playerPos[0])) { return false; }
      if (!stream.serialize(m_playerPos[1])) { return false; }
      if (!stream.serialize(m_playerPos[2])) { return false; }

      if (!stream.serialize(m_player2Pos[0])) { return false; }
      if (!stream.serialize(m_player2Pos[1])) { return false; }
      if (!stream.serialize(m_player2Pos[2])) { return false; }

      if (!stream.serialize(m_ufoPos[0])) { return false; }
      if (!stream.serialize(m_ufoPos[1])) { return false; }

      if (!stream.serialize(m_ufoAlive)) { return false; }

      if (!stream.serialize(m_score)) { return false; }
      if (!stream.serialize(m_lives)) { return false; }
      if (!stream.serialize(m_wave)) { return false; }
      if (!stream.serialize(m_running)) { return false; }
      if (!stream.serialize(m_win)) { return false; }
      if (!stream.serialize(m_loading)) { return false; }

      return true;
   }

   uint8  m_type{ 0 };
   uint32 m_tick{ 0 };
   uint32 m_sequence{ 0 };
   uint32 m_acknowledge{ 0 };

   //remove
   bool m_keys_down[4];
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
};
