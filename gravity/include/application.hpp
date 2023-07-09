// application.hpp

#pragma once

#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "client.hpp"
#include "Game.hpp"
#include "Interpolator.hpp"
#include "Inputinator.hpp"

class application final : client::listener {
public:
   application();

   void run();

private:
   bool enter();
   void exit();
   bool update();
   void render();

private:
   void on_closing();
   void on_mouse_moved(int x, int y);
   void on_key_pressed(sf::Keyboard::Key key);
   void on_key_released(sf::Keyboard::Key key);
   void on_button_pressed(sf::Mouse::Button button);
   void on_button_released(sf::Mouse::Button button);

private: // client::listener impl
   void on_connect();
   void on_disconnect(const bool timeout);
   void on_send(uint32 sequence, byte_stream_writer &writer);
   void on_receive(uint32 sequence, byte_stream_reader &reader);

private:
   sf::RenderWindow m_window;
   sf::Clock        m_clock;
   sf::Clock        m_playerclock;
   sf::Time         m_apptime;
   sf::Time         m_sendrate;
   sf::Time         m_deltatime;
   sf::Time         m_playerdeltatime;
   sf::Font         m_font;
   sf::Time         m_simulationrate;
   sf::Time         m_findserverrate;

private:
   bool             m_running{ true };
   client           m_client;
   keyboard         m_keyboard;
   uint32           m_tick{ 0 };
   uint32           m_server_tick{ 0 };
   sf::Text         m_server;
   sf::Text         m_usage;
   sf::Text         m_state;
   sf::Text         m_debug;

private:
    network          m_network;
    udp_socket       m_socket;
    udp_socket       m_discoverysocket;
    ip_address       m_serveraddress;
    ip_address       broadcastAddress;
    int              m_sequence{ 0 };

    Game* game{ nullptr };
    Interpolator m_interpolator;
    Inputinator m_inputinator;
    const float simRate{ 1.0f/60.0f };

    bool foundServer{ false };
};
