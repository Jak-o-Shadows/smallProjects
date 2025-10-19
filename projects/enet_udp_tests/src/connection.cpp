#include "connection.hpp"
#include <iostream>

bool Connection::Connect(const std::string& multicast_address_str, short multicast_port) {
    try {
        auto listen_address = asio::ip::make_address("0.0.0.0");
        auto multicast_address = asio::ip::make_address(multicast_address_str);

        // Endpoint for receiving
        asio::ip::udp::endpoint listen_endpoint(listen_address, multicast_port);

        // Endpoint for sending
        m_endpoint = asio::ip::udp::endpoint(multicast_address, multicast_port);

        m_socket.open(listen_endpoint.protocol());
        m_socket.set_option(asio::ip::udp::socket::reuse_address(true));
        m_socket.bind(listen_endpoint);

        // Join the multicast group.
        m_socket.set_option(asio::ip::multicast::join_group(multicast_address));

        std::cout << "Successfully joined multicast group " << multicast_address_str << ":" << multicast_port << std::endl;
        return true;
    } catch (const std::system_error& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        return false;
    }
}

void Connection::Disconnect() {
    if (m_socket.is_open()) {
        m_socket.close();
        std::cout << "Disconnected from multicast group." << std::endl;
    }
}

void Connection::Send(const char* data, size_t data_length) {
    m_socket.send_to(asio::buffer(data, data_length), m_endpoint);
}

std::vector<char> Connection::Receive() {
    std::vector<char> buffer(1024);
    asio::ip::udp::endpoint sender_endpoint;
    
    size_t length = m_socket.receive_from(asio::buffer(buffer), sender_endpoint);
    
    buffer.resize(length);
    return buffer;
}