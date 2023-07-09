// client.hpp

#pragma once

#include <shared.hpp>

struct client {
   struct listener {
      virtual ~listener() = default;
      virtual void on_connect() = 0;
      virtual void on_disconnect(const bool timeout) = 0;
      virtual void on_send(uint32 sequence, byte_stream_writer &writer) = 0;
      virtual void on_receive(uint32 sequence, byte_stream_reader &reader) = 0;
   };

   client(listener &listener);

   bool create();
   void destroy();

   void connect(const ip_address &address, uint32 session);
   void disconnect();

   void update();
   void transmit();

   bool is_disconnected() const;
   bool is_connected() const;
   bool is_connecting() const;
   bool is_disconnecting() const;

   timespan get_rtt() const;

private:
   void receive();
   bool is_from_server(const ip_address &address) const;
   void perform_timeout_check();

   void handle_connect(const ip_address &address, byte_stream_reader &reader);
   void handle_payload(const ip_address &address, byte_stream_reader &reader);
   void handle_disconnect(const ip_address &address, byte_stream_reader &reader);

   void send_disconnect();
   bool send_connecting();
   bool send_payload();
   bool send_byte_stream(const byte_stream &stream);

private:
   listener  &m_listener;
   network    m_network;
   udp_socket m_socket;
   connection m_connection;

};
