// application.cpp

#include "application.hpp"
#include "utility.hpp"

struct input_packet
{
    input_packet() {};
    input_packet(int seq_number, float time_sent, bool keys_pressed[int(sf::Keyboard::KeyCount)])
    {
        m_seq_number = seq_number;
        m_time_sent = time_sent;
        for (int i = 0; i < int(sf::Keyboard::KeyCount); i++)
        {
            m_keys_pressed[i] = keys_pressed[i];
        }
    }
    int m_seq_number{ -1 };
    float m_time_sent{ -1 };
    bool m_keys_pressed[int(sf::Keyboard::KeyCount)]{};
};

static ip_address 
get_same_host_server_address()
{
   std::vector<ip_address> addresses;
   if (!ip_address::get_local_addresses(addresses)) {
      return {};
   }

   //Changed addr.a() == 10 to addr.a() == 192
   for (auto &addr : addresses) {
      if (addr.a() == 192) {
         return ip_address{ addr.m_host, kDefaultServerPort };
         break;
      }
   }
   
   return {};
}

application::application()
   : m_client(*this)
{
}

bool application::enter()
{
   if (!m_font.loadFromFile("assets/proggy_tiny.ttf")) {
      return false;
   }
   
   if (!m_client.create()) {
      return false;
   }

   m_server.setFont(m_font);
   m_server.setCharacterSize(32);
   m_server.setPosition({ 8.0f, 2.0f });

   m_usage.setString("(C)onnect (D)isconnect");
   m_usage.setFont(m_font);
   m_usage.setCharacterSize(32);
   m_usage.setPosition({ 8.0f, 0.0f });

   m_state.setString("Disconnected");
   m_state.setFillColor(sf::Color::Red);
   m_state.setFont(m_font);
   m_state.setCharacterSize(32);
   m_state.setPosition({ 8.0f, 20.0f });

   m_debug.setFont(m_font);
   m_debug.setCharacterSize(16);

   m_socket.open();

   if (!m_discoverysocket.open())
   {

   }
   ip_address tempbroadcastAddress = get_same_host_server_address();
   broadcastAddress = ip_address(tempbroadcastAddress.a(), tempbroadcastAddress.b(), tempbroadcastAddress.c(), 255, 54321);

   foundServer = false;

   game = new Game(m_keyboard);
   
   return true;
}

void application::exit()
{
   m_client.disconnect();
}

void write(byte_stream& stream, int idNumber)
{
    byte_stream_writer writer = byte_stream_writer(stream);
    writer.serialize(idNumber);
}

bool application::update()
{
   m_deltatime = m_clock.restart();
   m_apptime += m_deltatime;
   m_simulationrate += m_deltatime;
   
   if (!foundServer)
   {
       m_findserverrate += m_deltatime;
       if (!m_client.is_connected() && m_findserverrate.asSeconds() >= 0.5)
       {
           m_findserverrate = sf::seconds(0.0f);
           byte_stream stream{};
           int connectCode = 1337;
           write(stream, connectCode);
           if (!m_discoverysocket.send(broadcastAddress, stream)) {
               
           }
       }

       ip_address address;
       byte_stream stream;
       int id;
       if (m_discoverysocket.receive(address, stream))
       {
           byte_stream_reader reader(stream);
           reader.serialize(id);
           if (id == 1337)
           {
               m_serveraddress = ip_address(address.a(), address.b(), address.c(), address.d(), 54321);
               foundServer = true;
           }
       }
       return true;
   }

   if (m_simulationrate.asSeconds() >= simRate)
   {
       m_simulationrate = sf::seconds(0.0f);
       m_interpolator.InterpolateEntities(m_apptime, game);
       if (m_client.is_connected())
       {
           m_playerdeltatime = sf::seconds(0.0f);
           m_playerdeltatime = m_playerclock.restart();
           game->Update(m_playerdeltatime.asSeconds());
           m_inputinator.addPrediction(m_tick, game->playerptr.position, m_keyboard.down(sf::Keyboard::I), m_keyboard.down(sf::Keyboard::J), m_keyboard.down(sf::Keyboard::L), game->playerptr.rotation);
           m_tick++;
       }
   }

   m_client.update();

   if (m_keyboard.pressed(sf::Keyboard::Key::C)) {
      if (m_client.is_disconnected()) {
         m_state.setFillColor(sf::Color::Cyan);
         m_state.setString("Connecting...");

         m_client.connect(m_serveraddress, 1);

         m_server.setString(m_serveraddress.as_string());
         auto size = m_window.getSize();
         auto rect = m_server.getLocalBounds();
         m_server.setPosition(size.x - rect.width - 10.0f, 2.0f);
      }

   }
   if (m_keyboard.pressed(sf::Keyboard::Key::D)) {
      if (m_client.is_connected()) {
         m_state.setFillColor(sf::Color::Cyan);
         m_state.setString("Disconnecting...");
         m_server.setString("");
         m_client.disconnect();
         m_tick = 0;
      }
   }

   { // debug text
      auto text = "Tick: " + std::to_string(m_tick) + " (" + std::to_string(m_server_tick) + ") ";
      text += "RTT:  " + std::to_string(m_client.get_rtt().elapsed_milliseconds()) + "ms";
      m_debug.setString(text);

      auto size = m_window.getSize();
      auto rect = m_debug.getLocalBounds();
      m_debug.setPosition({ 2.0f, size.y - rect.height - m_font.getLineSpacing(16) });
   }

   m_client.transmit();
   m_keyboard.update();

   return m_running;
}

void application::render()
{
   m_window.clear(sf::Color{ 0x55,0x55,0x55,0xff });
   m_window.draw(m_usage);
   m_window.draw(m_state);
   m_window.draw(m_server);
   m_window.draw(m_debug);
   if (m_client.is_connected())
   {
       game->Draw(m_window);
   }
   m_window.display();
}

void application::on_closing()
{
   m_running = false;
}

void application::on_mouse_moved(int x, int y)
{
}

void application::on_key_pressed(sf::Keyboard::Key key)
{
   if (key == sf::Keyboard::Key::Escape) {
      m_running = false;
   }

   m_keyboard.m_keys[int(key)].m_down = true;
   m_keyboard.m_keys[int(key)].m_pressed = true;
   m_keyboard.m_keys[int(key)].m_released = false;
}

void application::on_key_released(sf::Keyboard::Key key)
{
   m_keyboard.m_keys[int(key)].m_down = false;
   m_keyboard.m_keys[int(key)].m_pressed = false;
   m_keyboard.m_keys[int(key)].m_released = true;
}

void application::on_button_pressed(sf::Mouse::Button button)
{
}

void application::on_button_released(sf::Mouse::Button button)
{
}

void application::on_connect()
{
   m_state.setFillColor(sf::Color::Green);
   m_state.setString("Connected!");
}

void application::on_disconnect(const bool timeout)
{
   m_state.setFillColor(sf::Color::Red);
   m_state.setString(timeout ? "Timedout" : "Disconnected");
   m_server.setString("");

   m_tick = 0;
}

void application::on_send(uint32 sequence, byte_stream_writer &writer)
{
    bool keys_down[4];
    keys_down[0] = m_keyboard.down(sf::Keyboard::Key::I);
    keys_down[1] = m_keyboard.down(sf::Keyboard::Key::J);
    keys_down[2] = m_keyboard.down(sf::Keyboard::Key::L);
    keys_down[3] = m_keyboard.down(sf::Keyboard::Key::Space);

    //remove if version 1 is incorrect
    m_inputinator.addInputPrediction(m_tick, m_keyboard.down(sf::Keyboard::I), m_keyboard.down(sf::Keyboard::J), m_keyboard.down(sf::Keyboard::L));

    server_info_message message(m_tick, keys_down);

   if (!message.write(writer)) {
      assert(!"could not write server_info_message!");
   }
}

void application::on_receive(uint32 sequence, byte_stream_reader &reader)
{
   while (reader.has_data()) {
      auto type = (message_type)reader.peek();
      if (type == message_type::server_info) {
         server_info_message message;
         if (!message.read(reader)) {
            assert(!"could not read server_info_message!");
         }

         m_server_tick = message.m_tick;
         
         for (int i = 0; i < 20; i++)
         {
             game->m_asteroidScale[i] = message.m_asteroidScale[i];
         }
         
         game->m_ufoAlive = message.m_ufoAlive;
         game->m_score = message.m_score;
         game->m_lives = message.m_lives;
         game->m_wave = message.m_wave;
         game->running = message.m_running;
         game->win = message.m_win;
         game->loading = message.m_loading;

         m_inputinator.CheckOffset(message, game);
         m_interpolator.add_packet(message, m_apptime);
      }
      else {
         assert(!"unknown message type!");
      }
   }
}
