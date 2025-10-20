#include "connection.hpp"
#include <iostream>

Connection::Connection()
    : socket(io_context) {
}

bool Connection::Connect(const std::string& broadcast_addr, short broadcast_port) {
    try {
        broadcast_endpoint = asio::ip::udp::endpoint(
            asio::ip::address::from_string(broadcast_addr),
            broadcast_port
        );

        socket.open(asio::ip::udp::v4());
        socket.set_option(asio::socket_base::broadcast(true));
        socket.set_option(asio::ip::udp::socket::reuse_address(true));

        asio::ip::udp::endpoint listen_endpoint(asio::ip::address_v4::any(), broadcast_port);
        socket.bind(listen_endpoint);

        // Set non-blocking
        asio::error_code ec;
        socket.non_blocking(true, ec);
        if (ec) {
            std::cerr << "Failed to set non-blocking: " << ec.message() << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void Connection::Disconnect() {
    if (socket.is_open()) {
        socket.close();
    }
}

void Connection::Send(const char* data, size_t data_length) {
    try {
        socket.send_to(asio::buffer(data, data_length), broadcast_endpoint);
    } catch (const std::exception& e) {
        std::cerr << "Send failed: " << e.what() << std::endl;
    }
}

std::vector<char> Connection::Receive() {
    std::vector<char> buffer(1500); // Standard MTU size for Ethernet
    try {
        size_t length = socket.receive_from(asio::buffer(buffer), sender_endpoint);
        buffer.resize(length);
        return buffer;
    } catch (const asio::system_error& e) {
        if (e.code() == asio::error::would_block) {
            // No data available, return empty vector
            return {};
        }
        std::cerr << "Receive failed: " << e.what() << std::endl;
        return {};
    } catch (const std::exception& e) {
        std::cerr << "Receive failed: " << e.what() << std::endl;
        return {};
    }
}