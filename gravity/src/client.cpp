// client.cpp

#include "client.hpp"

client::client(listener &listener) 
   : m_listener(listener)
{
}

bool client::create()
{
   if (!m_socket.open()) {
      return false;
   }

   return true;
}

void client::destroy()
{
   m_socket.close();
}

void client::connect(const ip_address &address, uint32 session)
{
   if (!m_connection.is_disconnected()) {
      return;
   }
   
   m_connection.set_state(connection::state::connecting);
   m_connection.set_session(session);
   m_connection.set_address(address);
}

void client::client::disconnect()
{
   if (!m_connection.is_connected()) {
      return;
   }

   m_connection.set_state(connection::state::disconnecting);
   m_connection.set_send_interval(kDefaultSendInterval);
   m_connection.set_disconnect_time(timespan::time_since_start());
}

void client::update()
{
    perform_timeout_check();
    receive();
}

void client::receive()
{
   ip_address address;
   byte_stream stream;
   if (m_socket.receive(address, stream)) {
      byte_stream_reader reader(stream);
      auto type = (protocol_packet_type)reader.peek();
      if (type == protocol_packet_type::connect) {
         handle_connect(address, reader);
      }
      else if (type == protocol_packet_type::disconnect) {
         handle_disconnect(address, reader);
      }
      else if (type == protocol_packet_type::payload) {
         handle_payload(address, reader);
      }
      else {
         assert(!"Invalid connection packet received!");
      }
   }
   else {
      auto error = network::get_last_error();
      if (error.is_critical()) {
         assert(!"Critical network error!");
      }
   }
}

void client::transmit()
{
   if (m_connection.is_disconnected()) {
      return;
   }

   const timespan current_time = timespan::time_since_start();
   if (m_connection.is_time_to_send()) {
      if (m_connection.is_connected()) {
         if (send_payload()) {
            m_connection.set_last_send(current_time);
         }
      }
      else if (m_connection.is_connecting()) {
         if (send_connecting()) {
            m_connection.set_last_send(current_time);
         }
      }
      else if (m_connection.is_disconnecting()) {
         const timespan disconnect_duration = current_time - m_connection.m_disconnect_time;
         if (disconnect_duration >= kDefaultDisconnectDuration) {
            m_listener.on_disconnect(false);
            m_connection.reset();
         }
         else {
            send_disconnect();
            m_connection.set_last_send(current_time);
         }
      }
   }
}

bool client::is_disconnected() const
{
   return m_connection.is_disconnected();
}

bool client::is_connected() const
{
   return m_connection.is_connected();
}

bool client::is_connecting() const
{
   return m_connection.is_connecting();
}

bool client::is_disconnecting() const
{
   return m_connection.is_disconnecting();
}

timespan client::get_rtt() const
{
   return m_connection.get_current_roundtrip_time();
}

bool client::is_from_server(const ip_address &address) const
{
   return m_connection.m_address == address;
}

void client::perform_timeout_check()
{
   if (m_connection.is_connected() && m_connection.is_timed_out()) {
      m_listener.on_disconnect(true);
      m_connection.reset();
   }
}

void client::handle_connect(const ip_address &address, byte_stream_reader &reader)
{
   if (!is_from_server(address)) {
      return;
   }

   connection_packet packet;
   if (!packet.read(reader)) {
      return;
   }

   if (m_connection.is_connecting()) {
      m_connection.set_last_recieved(timespan::time_since_start());
      if (m_connection.m_session == packet.m_session) {
         m_listener.on_connect();
         m_connection.set_state(connection::state::connected);
         m_connection.set_send_interval(kDefaultConnectedSendInterval);
      }
      else {
         m_listener.on_disconnect(false);
         m_connection.set_state(connection::state::disconnected);
         m_connection.set_send_interval(kDefaultSendInterval);
      }
   }
}

void client::handle_payload(const ip_address &address, byte_stream_reader &reader)
{
   if (!is_from_server(address)) {
      return;
   }

   payload_packet packet;
   if (!packet.read(reader)) {
      return;
   }

   if (m_connection.is_connected()) {
      if (m_connection.m_acknowledge < packet.m_sequence) {
         const timespan process_time(int64(packet.m_process_time_us));
         const timespan send_time = m_connection.m_rtt_buffer.get(packet.m_acknowledge);
         const timespan roundtrip = timespan::time_since_start() - send_time - process_time;

         m_connection.set_roundtrip_time(roundtrip);
         m_connection.set_acknowledge(packet.m_sequence);
         m_connection.set_last_recieved(timespan::time_since_start());
         m_listener.on_receive(packet.m_acknowledge, reader);
      }
   }
}

void client::handle_disconnect(const ip_address &address, byte_stream_reader &reader)
{
   if (!is_from_server(address)) {
      return;
   }

   disconnect_packet packet;
   if (!packet.read(reader)) {
      return;
   }

   if (m_connection.is_disconnecting()) {
      m_listener.on_disconnect(false);
      m_connection.reset();
   }
   else if (m_connection.is_connected()) {
      m_connection.set_state(connection::state::disconnecting);
      m_connection.set_send_interval(kDefaultSendInterval);
      m_connection.set_disconnect_reason(packet.m_reason);
      m_connection.set_disconnect_time(timespan::time_since_start());
   }
}

void client::send_disconnect()
{
   byte_stream stream;
   byte_stream_writer writer(stream);
   disconnect_packet packet((disconnect_reason_type)m_connection.m_disconnect_reason);
   if (packet.write(writer)) {
      if (!send_byte_stream(stream)) {
         // note: do we care that if failed?
      }
   }
}

bool client::send_connecting()
{
   byte_stream stream;
   byte_stream_writer writer(stream);
   connection_packet packet(m_connection.m_session);
   if (packet.write(writer)) {
      if (send_byte_stream(stream)) {
         return true;
      }
   }

   return false;
}

bool client::send_payload()
{
   const timespan current_time = timespan::time_since_start();
   m_connection.m_rtt_buffer.set(m_connection.m_sequence, current_time);

   const timespan process_time = current_time - m_connection.m_last_receive_time;
   const uint32 process_time_us = uint32(process_time.elapsed_microseconds());

   byte_stream stream;
   byte_stream_writer writer(stream);
   payload_packet packet(m_connection.m_sequence,
                         m_connection.m_acknowledge,
                         process_time_us);
   if (packet.write(writer)) {
      m_listener.on_send(m_connection.m_sequence, writer);
      m_connection.increment_sequence();
      if (send_byte_stream(stream)) {
         return true;
      }
   }

   return false;
}

bool client::send_byte_stream(const byte_stream &stream)
{
   if (!m_socket.send(m_connection.m_address, stream)) {
      auto error = network::get_last_error();
      if (error.is_critical()) {
         assert(!"Critical network error!");
      }

      return false;
   }

   return true;
}
