// server.cpp

#include "server.hpp"

server::server(listener &listener)
   : m_listener(listener)
{
}

bool server::create(const ip_address &address)
{
   if (!m_socket.open_and_bind(address)) {
      return false;
   }

   m_serverdiscsocket.open();

   return true;
}

void server::destroy()
{
   m_socket.close();
}

void server::update()
{
   perform_timeout_check();
   remove_disconnected_connections();
   receive();
}

void server::receive()
{
   int receive_loop_guard = 8;
   while (receive_loop_guard) {
      receive_loop_guard--;

      ip_address address;
      byte_stream stream;
      int id;
      if (m_socket.receive(address, stream)) {
         byte_stream_reader reader(stream);
         const auto packet_type = (protocol_packet_type)reader.peek();
         if (packet_type == protocol_packet_type::connect) {
            handle_connect(address, reader);
         }
         else if (packet_type == protocol_packet_type::disconnect) {
            handle_disconnect(address, reader);
         }
         else if (packet_type == protocol_packet_type::payload) {
            handle_payload(address, reader);
         }
         
         else {

             reader.serialize(id);
             if (id == 1337)
             {
                 handle_findserver(address, reader);
             }
             else
             {
                 assert(!"Invalid connection packet received!");
                 break;
             }
         }
      }
      else {
         auto error = network::get_last_error();
         if (error.is_critical()) {
            assert(!"Critical network error!");
            break;
         }
      }
   }
}

void server::transmit()
{
   const timespan current_time = timespan::time_since_start();
   for (auto &conn : m_connections) {
      if (conn.is_time_to_send()) {
         if (conn.is_connecting()) {
            if (send_connecting(conn)) {
               conn.set_last_send(current_time);
            }
         }
         else if (conn.is_disconnecting()) {
            const timespan disconnect_duration = current_time - conn.m_disconnect_time;
            if (disconnect_duration >= kDefaultDisconnectDuration) {
               m_listener.on_disconnect(conn.m_id, false);
               conn.reset();
            }
            else {
               if (send_disconnect(conn.m_address, conn.m_disconnect_reason)) {
                  conn.set_last_send(current_time);
               }
            }
         }
         else if (conn.is_connected()) {
            if (send_payload(conn)) {
               conn.set_last_send(current_time);
            }
         }
      }
   }
}

void server::perform_timeout_check()
{
   for (auto &conn : m_connections) {
      if (conn.is_timed_out()) {
         m_listener.on_disconnect(conn.m_id, true);
         conn.set_state(connection::state::disconnected);
      }
   }
}

void server::remove_disconnected_connections()
{
   for (auto it = m_connections.begin(); it != m_connections.end(); ) {
      if ((*it).is_disconnected()) {
         m_listener.on_disconnect((*it).m_id, false);
         it = m_connections.erase(it);
         continue;
      }

      ++it;
   }
}

bool server::contains(const ip_address &address) const
{
   for (auto &conn : m_connections) {
      if (conn.m_address == address) {
         return true;
      }
   }

   return false;
}

connection *server::add_connecting(const ip_address &address, const uint32 session)
{
   connection conn(address);
   conn.set_id(++m_connection_counter);
   conn.set_state(connection::state::connecting);
   conn.set_session(session);
   conn.set_last_recieved(timespan::time_since_start());
   m_connections.push_back(conn);
   return &m_connections.back();
}

void server::remove_connection(const ip_address &address)
{
   for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
      if ((*it).m_address == address) {
         m_connections.erase(it);
         break;
      }
   }
}

connection *server::get_connection(const ip_address &address)
{
   for (auto &conn : m_connections) {
      if (conn.m_address == address) {
         return &conn;
      }
   }

   return nullptr;
}

void server::handle_connect(const ip_address &address, byte_stream_reader &reader)
{
   connection_packet packet;
   if (!packet.read(reader)) {
      return;
   }
    
   if (packet.m_protocol != kProtocolIdent) {
      send_disconnect(address, uint32(disconnect_reason_type::wrong_ident));
      return;
   }

   if (packet.m_version != kProtocolVersion) {
      send_disconnect(address, uint32(disconnect_reason_type::wrong_version));
      return;
   }

   if (!contains(address)) {
      (void)add_connecting(address, packet.m_session);
   }
}

void server::handle_disconnect(const ip_address &address, byte_stream_reader &reader)
{
   disconnect_packet packet;
   if (!packet.read(reader)) {
      return;
   }

   if (connection *conn = get_connection(address)) {
      if (conn->is_connected()) {
         const timespan current_time = timespan::time_since_start();
         conn->set_state(connection::state::disconnecting);
         conn->set_last_recieved(current_time);
         conn->set_disconnect_time(current_time);
         conn->set_disconnect_reason(packet.m_reason);
      }
      else if (conn->is_disconnecting()) {
         conn->set_state(connection::state::disconnected);
         conn->set_last_recieved(timespan::time_since_start());
      }
   }
}

void server::handle_payload(const ip_address &address, byte_stream_reader &reader)
{
   payload_packet packet;
   if (!packet.read(reader)) {
      return;
   }

   if (connection *conn = get_connection(address)) {
      if (conn->is_connecting()) {
         m_listener.on_connect(conn->m_id);
         conn->set_state(connection::state::connected);
      }

      if (conn->m_acknowledge < packet.m_sequence) {
         const timespan process_time(int64(packet.m_process_time_us));
         const timespan send_time = conn->m_rtt_buffer.get(packet.m_acknowledge);
         const timespan roundtrip = timespan::time_since_start() - send_time - process_time;

         conn->set_roundtrip_time(roundtrip);
         conn->set_last_recieved(timespan::time_since_start());
         conn->set_acknowledge(packet.m_sequence);
         m_listener.on_receive(conn->m_id, packet.m_acknowledge, reader);
      }
   }
}

bool server::send_connecting(const connection &conn)
{
   byte_stream stream;
   byte_stream_writer writer(stream);
   connection_packet packet(conn.m_session);
   if (packet.write(writer)) {
      if (send_byte_stream(conn.m_address, stream)) {
         return true;
      }
   }

   return false;
}

bool server::send_disconnect(const ip_address &address, uint32 reason)
{
   byte_stream stream;
   byte_stream_writer writer(stream);
   disconnect_packet packet((disconnect_reason_type)reason);
   if (packet.write(writer)) {
      if (send_byte_stream(address, stream)) {
         return true;
      }
   }

   return false;
}

bool server::send_payload(connection &conn)
{
   const timespan current_time = timespan::time_since_start();
   conn.m_rtt_buffer.set(conn.m_sequence, current_time);

   const timespan process_time = current_time - conn.m_last_receive_time;
   const uint32 process_time_us = uint32(process_time.elapsed_microseconds());

   byte_stream stream;
   byte_stream_writer writer(stream);
   payload_packet packet(conn.m_sequence, conn.m_acknowledge, process_time_us);
   if (packet.write(writer)) {
      m_listener.on_send(conn.m_id, conn.m_sequence, writer);
      if (send_byte_stream(conn.m_address, stream)) {
         conn.increment_sequence();
         return true;
      }
   }

   return true;
}

bool server::send_byte_stream(const ip_address &address, const byte_stream &stream)
{
   if (!m_socket.send(address, stream)) {
      auto error = network::get_last_error();
      if (error.is_critical()) {
         assert(!"Critical network error!");
      }
      return false;
   }

   return true;
}

void write(byte_stream& stream, int idNumber)
{
    byte_stream_writer writer = byte_stream_writer(stream);
    writer.serialize(idNumber);
}

void server::handle_findserver(const ip_address& address, byte_stream_reader& reader)
{
    byte_stream stream{};
    write(stream, 1337);
    m_serverdiscsocket.send(address, stream);
    int a = 1;
}

